# Rett maskinavhengig CMake-cache i Git

- [x] Undersøk vanlig CMake-bygg og CI sitt byggeoppsett uten å kjøre verifikatoren.
- [x] Finn genererte byggeartefakter som er sporet av Git.
- [x] Gjenskap CMake-feilen ved å bruke cachen fra en annen byggesti.
- [x] Bygg bibliotek, demo og fire scenarioer fra en ny CMake-konfigurasjon.
- [x] Fjern project/build fra Git-indeksen, og behold lokale byggeartefakter.

Årsak: project/build/CMakeCache.txt var sjekket inn med absolutte stier til
/home/hoglar/modelering/lab1/specDrivenDev. CI bruker en annen arbeidsmappe.
En kopi av cachen i en annen byggemappe ga CMake-feilen «different than the
directory ... where CMakeCache.txt was created» og exitkode 1.

Bygging fra bunnen med `cmake -S project/impl -B project/build-fix-clean` og
`cmake --build project/build-fix-clean` fullførte med exitkode 0, uten advarsler.
De midlertidige byggemappene for denne undersøkelsen er fjernet etterpå.

Rettelse: `git rm -r --cached --quiet -- project/build`. Dette klargjør fjerning
av genererte filer fra Git, uten å slette brukerens lokale byggefiler.
Eksisterende .gitignore har allerede build/; ingen ny ignore-regel er nødvendig.
C++-koden, inputdiagrammene og workflowen er uendret.

Fjerningen må committes og pushes før GitHub Actions får en ren checkout.
CI er ikke kjørt på nytt her. Verifikatoren og sporingsbygg er ikke kjørt;
genererte verifikasjonsrapporter er ikke lest.
