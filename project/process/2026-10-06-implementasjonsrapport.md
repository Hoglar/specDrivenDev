# Implementasjonsrapport — Open Game Library

Dato: 6. oktober 2026.
Status: **Byggbar og kjørbar C++20-mock-up levert. 100 % alignment er ikke oppnådd.**

## Leveranse

Implementasjonen ligger i `project/impl/`: CMake, åtte headers, åtte
klassekilder, `src/demo.cpp` og `scenarios/user.cpp`.
De sju klassene fra klassediagrammet er implementert, sammen med
sekvensens controller `game_page`, som er en dokumentert modellforskjell.

Planen og specen er oppdatert for automatisk gjennomføring uten spørsmål
eller godkjenningsstopp. Valgte tolkninger står i
[ADR 0001](adr/0001-automatisk-uml-tolkning.md).
Originaldiagrammene og filnavnene er bevart, kontrollert med SHA-256.
Ingen commit eller push er utført.

## Bygg og kjør

Fra repoets rot:

```bash
cmake -S project/impl -B project/build -DCMAKE_BUILD_TYPE=Debug
cmake --build project/build
project/build/demo
project/build/scenario_user
```

Begge programmene avsluttet med exit-kode 0. Demoen skriver
`C++ UML mock-up: A deterministic review`. Assertions kontrollerer
anmeldelsens innhold og repository-metadata som returneres ved download.

## Kontroller ved første implementasjon

- Bibliotek, demo og scenario bygget med GCC 14.2, C++20, `-Wall -Wextra`,
  uten compiler-warnings.
- Separat bygg fra ren mappe med AddressSanitizer og UndefinedBehaviorSanitizer:
  begge programmer kjørt uten funn, inkludert LeakSanitizer. Sandkassen
  blokkerte først LeakSanitizer; kontrollen ble gjentatt utenfor sandkassen.
- Standardkommandoen `.venv/bin/python tools/verify.py project` avslutter
  fortsatt med exit-kode 1: originalnavnene firstClass og gameReviewSequence
  støttes ikke. User-state leses og sammenlignes.
- Alle tre diagrammer er i tillegg kontrollert i en midlertidig mappe med
  byte-identiske kopier og støttede filnavn. Verifier og originalfiler er
  ikke endret for dette. Kommando:

```bash
.venv/bin/python project/process/verify_readonly_input.py
```

| Modell | Identiske | Missing | Extra | Alignment |
|---|---:|---:|---:|---:|
| Klasse | 33 | 8 | 18 | 55,9 % |
| User-state | 14 | 1 | 5 | 70,0 % |
| Anmeldelsessekvens | 4 | 2 | 1 | 57,1 % |

Alle har 0 Changed. Verifierens exit-kode 0 for kopikontrollen betyr at
rapporter ble produsert, **ikke** at modellene er 100 % aligned.
Rapportene finnes under [verification/reports/](verification/reports/).
Genererte Mermaid-filer, sammenligningsdiagrammer og kopier av input er
bevart under verification/diagrams/. Dette er kontrollkopier, ikke ny fasit.
Rapportenes lenke til impl/include gjelder det midlertidige prosjektet;
de aktuelle headerne finnes i `project/impl/include/game_library/`.

## Hvorfor alignment ikke er 100 %

1. Åtte assosiasjoner mangler rollenavn i klassediagrammet. C++-medlemmene
   har nødvendige navn, og verifieren matcher rolle som del av identiteten.
   Derfor rapporteres både Missing uten rolle og Extra med rolle, selv om
   type, retning og samling er implementert etter leserens tolkning.
2. User-maskinen krever state/handle/guard og data for sperreflagget som
   ikke står i klasseboksen. Disse er ekstra klasseelementer.
3. Repository.download er tegnet uten synlighet og hoppes over av readeren.
   Den implementerte public-metoden rapporteres derfor som Extra.
4. game_page er tegnet i sekvensen, men ikke i klassediagrammet. Controlleren,
   dens submit_review og signaturavhengigheter rapporteres som Extra.
5. «Send inn review» er ikke et C++-metodenavn. Tolkningen submit_review
   rapporteres som ekstra sekvensmelding, og den opprinnelige som Missing.
6. Game_Review finnes bare i sekvensen, mens Review er klassen som tegnes.
   Lifeline Game_Review rapporteres som Missing. Konstruktørkallet til Review
   registreres ikke som en sekvensmelding, etter verifierens regler.
7. Gjenåpningsetiketten er løs tekst, og tidsmerket pil til sluttsymbol
   støttes ikke. ReopenAccount med guard og After30Days → Deleted er derfor
   rapporterte tolkninger, ikke identiske overganger mot readerens modell.

## Kvalitet og begrensninger

Koden er liten, med én namespace, én header per klasse og eksplisitte typer.
Overgangene er samlet i en tabell. Uklare steder er kommentert og koblet til
ADR-en. Ingen database, grafikk, faktisk nedlasting eller nye produktregler.

Assosiasjonssamlinger bruker weak_ptr uten å eie objektene. Enkelte råpekere
forutsetter at refererte objekter lever lenge nok. Demoens objekter følger
denne rekkefølgen. Repository kontrollerer at minst én bidragsyter er i live
ved konstruksjon; løpende håndheving av dette er ikke modellert.

Opprettelsesmetoder returnerer verdier uten automatisk registrering i
assosiasjonssamlingene. create_repository, join_repository og remove_comment
er dokumenterte plassholdere. Ingen pekere til midlertidige returverdier
lagres. Forfatterparametere uten tilsvarende relasjon på mottakerklassen
lagres ikke som ekstra data. Dato ved opprettelse av anmeldelse er fast epoch;
demoens utgivelsesdato er fast 2026-10-06.

State-tabellen er kontrollert strukturelt, men det er ikke utført en full
runtime-test av alle private tilstander og begge guardutfall. Ingen ekstra
getter er lagt til bare for testing. Minnekontrollen gjelder de kjørte
demoforløpene, ikke alle mulige bruksmønstre.

Planen beholdes i `plans/`, siden kravene om 100 % alignment og full
tilstandskontroll fortsatt står åpne. Dette rapporteres uten flere spørsmål;
inputmodellene er ikke endret for å forbedre resultatet.

## Oppfølging: sequence-user

Brukeren har gitt inputfilene navnene `class.drawio` og `sequence-user.drawio`.
Scenariokilden er derfor flyttet til `scenarios/user.cpp`, og CMake bygger
`scenario_user`. AI-en har ikke endret input-diagrammene.

Standardkommandoen `tools/verify.py project` produserer nå alle tre rapporter
og avslutter med exit-kode 0. Alignment er fortsatt 55,9 % / 57,1 % / 70,0 %
for klasse / sekvens / state. Sekvensen kjøres og spores korrekt; resterende
avvik er Game_Review-lifeline og «Send inn review» versus submit_review.
De nye rapportene ligger i `project/reports/`.

## Oppfølging: metodekall matcher

Controller-metoden er endret fra submit_review til `send_inn_review`, som
matcher «Send inn review» etter verifierens dokumenterte navnenormalisering.
Demo og scenario er oppdatert og kjørt. Inputfilenes SHA-256 er uendret.

Ny standardverifikasjon gir **83,3 % sekvensalignment**: fem identiske,
én Missing og ingen Extra/Changed. Begge kallene har riktig sender,
mottaker, metodenavn og rekkefølge. Klasse/state er fortsatt 55,9 % / 70,0 %.

Den gjenværende Missing er lifeline Game_Review. Runtime-verifikatoren
utelater konstruktører og oppretter bare lifelines for deltakere i vanlige
metodekall. Denne lifelinen kan derfor ikke registreres fra create-pilen
alene, selv om Review faktisk opprettes. Å innføre et ekstra metodekall
eller en ekstra Game_Review-klasse ville gi nye avvik fra input.
Ingen slik endring av modell eller verifier er utført.
