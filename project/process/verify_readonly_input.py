"""Verify byte-identical input copies with supported names; never change originals.

Run with: .venv/bin/python project/process/verify_readonly_input.py
Separate reports are saved in project/process/verification/.
Verifier exit status does not imply 100 percent alignment; read every report.
"""
from pathlib import Path
import hashlib
import shutil
import subprocess
import sys
import tempfile


def main():
    repo = Path(__file__).resolve().parents[2]
    project = repo / "project"
    sources = {
        "firstClass.drawio": "class.drawio",
        "gameReviewSequence.drawio": "sequence-write_review.drawio",
        "state-user.drawio": "state-user.drawio",
    }
    digests = {
        name: hashlib.sha256((project / "diagrams/input" / name).read_bytes()).hexdigest()
        for name in sources
    }
    destination = project / "process/verification"
    destination.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="game-library-verify-") as directory:
        staging = Path(directory)
        inputs = staging / "diagrams/input"
        inputs.mkdir(parents=True)
        for source, target in sources.items():
            original = project / "diagrams/input" / source
            copy = inputs / target
            shutil.copyfile(original, copy)
            assert hashlib.sha256(copy.read_bytes()).hexdigest() == digests[source]
        shutil.copytree(project / "impl", staging / "impl")
        result = subprocess.run([sys.executable, str(repo / "tools/verify.py"), str(staging)],
                                cwd=repo, capture_output=True, text=True)
        print(result.stdout, end="")
        print(result.stderr, end="", file=sys.stderr)
        (destination / "verifier.log").write_text(result.stdout + result.stderr)
        for pattern in ("reports/*-report.md", "diagrams/output/*", "diagrams/input/*.drawio"):
            for artifact in staging.glob(pattern):
                target = destination / artifact.relative_to(staging)
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(artifact, target)
        # Original paths stay untouched, including their file names.
        for name, digest in digests.items():
            assert hashlib.sha256((project / "diagrams/input" / name).read_bytes()).hexdigest() == digest
        print("Original input files unchanged. Reports:", destination)
        return result.returncode


if __name__ == "__main__":
    raise SystemExit(main())
