# Plan for implementasjon av UML-designet

## Instruks til AI-agenten

Planen beskriver hvordan UML-diagrammene skal implementeres i C++20 med
`project/process/specs/specs.md` som ramme for arbeidet.

Når oppdragsgiver ber deg følge denne planen, skal du gjennomføre hele
implementasjonen beskrevet nedenfor. Beskjeden er klarsignal til å lese kildene,
dokumentere designet, skrive C++-koden og bygge den. Ikke stopp etter å ha
oppsummert planen, skrevet dokumentasjon eller opprettet tomme filer. Du trenger
ikke tidligere samtalehistorikk for å utføre oppgaven.

Alle stier er relative til repoets rot, der `AGENTS.md` ligger. Les filene på disk
ved oppstart; diagrammene kan ha blitt oppdatert siden denne planen ble skrevet.
Spesifikasjonen og `AGENTS.md` angir arbeidsreglene, mens inputdiagrammene angir
det konkrete designet. Listen over observerte uklarheter nedenfor er arbeidsnotater,
ikke vedtatte løsninger eller ekstra krav.

Verifisering utføres uavhengig av oppdragsgiver etter implementasjonen og inngår
ikke i oppdraget. Vanlig kompilering med CMake er tillatt.

### Gjennomføring og fremdrift

- Gjennomfør steg 1–7 i rekkefølge, og fortsett mellom stegene uten å be om
  godkjenning for ordinær dokumentasjon, implementasjon eller bygging.
- Bruk denne planen som arbeidsliste; ikke opprett en ny plan i stedet for å
  implementere. Det avtalte filnavnet for denne oppgaven er `plan.md`.
- Kryss av et punkt først når arbeidet er utført. Før korte fremdriftsnotater
  nederst i filen, med fullførte deler, neste steg og eventuelle blokkeringer.
- Ved gjenopptakelse: les notatene og eksisterende implementasjon før du
  fortsetter. Bevar brukerens arbeid og bygg videre på det som allerede finnes.
- Ta ordinære implementasjonsvalg selv innenfor UML-designet og mappingreglene.
  Ikke be om tillatelse til hvert steg. Hvis en reell mangel eller motstrid i
  designet krever et nytt funksjons- eller designvalg, still et konkret spørsmål
  og fortsett med uavhengige deler mens du venter. Ikke gjett deg til nye krav.
- Lever fungerende metodeimplementasjoner. Ikke bruk tomme metodekropper,
  midlertidige returverdier eller TODO-er som erstatning for spesifisert oppførsel.

Oppdraget er ferdig når alle avklarte deler av designet er implementert, demo og
scenario-programmer er på plass, og hele implementasjonen bygger uten advarsler.
Hvis nødvendige krav fortsatt er uavklart, oppgi hva som gjenstår og ikke marker
oppdraget som fullført.

## 1. Les spesifikasjonen og diagrammene

- [x] Les `AGENTS.md` og `project/process/specs/specs.md` ved oppstart.
- [x] Les `project/process/specs/adr.md` som konseptbakgrunn, uten å utlede nye
  funksjonskrav fra den.
- [x] Les følgende filer direkte fra `project/diagrams/input/`:
  - `class.drawio`
  - `state-game.drawio`, `state-request.drawio` og `state-user.drawio`
  - `sequence-nedlastning.drawio`, `sequence-publish.drawio`,
    `sequence-request.drawio` og `sequence-review.drawio`
- [x] Finn også eventuelle nye `state-*.drawio` og `sequence-*.drawio` i
  inputmappen, og ta dem med i samme arbeidsflyt. Les XML, etiketter,
  pilretninger og tilkoblinger direkte; ikke bruk verifikatoren til konvertering.
- [x] Les UML-til-C++-mappingen som `AGENTS.md` viser til.

Inputdiagrammene skal ikke endres. Genererte diagramutdata og
verifikasjonsrapporter skal ikke brukes som grunnlag for arbeidet.

## 2. Dokumenter hva som skal implementeres

- [x] Dokumenter klassene, enum-typene, attributtene, metodene, synligheten,
  relasjonene og multiplisitetene i `project/process/specs/design.md`.
  Behold `specs.md` som arbeidsregler og `adr.md` som konseptbakgrunn.
- [x] Angi C++-konstruksjonen som hvert designelement skal oversettes til.
- [x] Dokumenter starttilstand, tilstander, hendelser, betingelser, handlinger
  og overganger for hver tilstandsmaskin.
- [x] Dokumenter deltakere, avsender, mottaker og rekkefølge på kallene i hvert
  sekvensdiagram. Skill mellom metodekall, konstruksjon og returmeldinger.
- [x] Noter nødvendige avklaringer og få dem avklart før de berørte delene
  implementeres. Fortsett med uavhengige deler som er tilstrekkelig beskrevet.

Følgende punkter ble observert ved første gjennomlesing og må avklares ved
detaljeringen:

- Typen `uuid` og flere metoders returtyper er ikke konkretisert som C++-typer.
- `Category` inneholder `...`; det er uklart om flere verdier skal inngå.
- `User_status` og `User_Status` har ulik skrivemåte. Tilstandsdiagrammene
  inneholder også tilstander som ikke står i klassediagrammets status-enumerasjoner,
  blant annet `Validation` for Game og `Closed` for User.
- Tilstandsmaskinenes hendelser og kobling til klassemetodene må konkretiseres
  der diagrammene mangler navn eller signaturer.
- Sekvensdiagrammene bruker blant annet `Developer Portal`, `game_page` og
  `Game_Review`, som ikke er klassenavn i klassediagrammet. Det må avklares hvilke
  deltakere som er eksterne aktører, og hvilke som skal representeres i koden.
- `add_review(author, content)` i sekvensdiagrammet har andre parametere enn
  `add_review(user, title, message)` i klassediagrammet.
- Alternative forløp i request-scenarioet må konkretiseres som et deterministisk
  scenario uten å behandle alternative grener som én sammenhengende kallrekke.

Ikke løs uklarheter ved å legge til egne klasser, metoder eller funksjonskrav.

## 3. Opprett implementasjonsstrukturen

- [x] Opprett `project/impl/CMakeLists.txt` for C++20 med
  `CMAKE_EXPORT_COMPILE_COMMANDS ON` og `-Wall -Wextra`.
- [x] Velg ett kort, lowercase prosjektnavn som namespace og mappe under
  `project/impl/include/`.
- [x] Bruk én header per klasse og `snake_case` i filnavn.
- [x] Legg metodeimplementasjonene i `project/impl/src/` og opprett `src/demo.cpp`.
- [x] Opprett et statisk bibliotek med offentlig include-mappe, og lenk demoen
  og scenario-programmene mot biblioteket. Hold programmene med `main()` utenfor
  bibliotekets kildefiler.

## 4. Implementer klassedesignet

- [x] Implementer enum-typene og klassene `User`, `Game`, `Comment`, `Review`,
  `Repository` og `Request` etter diagrammet og avklaringene.
- [x] Deklarer medlemmer og relasjoner i headerne med tegnede navn, typer og
  synlighet.
- [x] Implementer arv, eierskap og multiplisitet med konstruksjonene som er
  angitt i mappingreglene.
- [x] Implementer metodekroppene uten ekstra funksjonalitet eller nye medlemmer.

## 5. Implementer tilstandsmaskinene

- [x] Implementer tilstandsmaskinene for `Game`, `Request` og `User` med mønsteret
  i `AGENTS.md`, etter at nødvendige avklaringer er dokumentert.
- [x] Bruk nestede tilstands- og hendelses-enumerasjoner og overgangstabeller.
- [x] Angi alle fem felt i hver overgang: fra, hendelse, til, betingelse og handling.
- [x] Sett starttilstanden og implementer hendelseshåndtering uten klokke eller
  venting i scenario-programmene.

## 6. Implementer scenario-programmene

- [x] Opprett `scenarios/nedlastning.cpp`, `scenarios/publish.cpp`,
  `scenarios/request.cpp` og `scenarios/review.cpp` under `project/impl/`.
- [x] La `main()` opprette objektene og kalle `scenario()`.
- [x] La `scenario()` utføre aktørens kall, og la klassemetodene utføre de
  tegnede kallene mellom klassene i riktig rekkefølge.
- [x] Hold scenarioene deterministiske, uten brukerinput, tilfeldighet, klokke
  eller flere tråder.
- [x] Legg til ett CMake-target med navnet `scenario_<scenario>` per program.

## 7. Bygg og lever implementasjonen

- [x] Konfigurer med `cmake -S project/impl -B project/build`.
- [x] Kompiler med `cmake --build project/build`.
- [x] Rett byggefeil og kompilatoradvarsler innenfor spesifikasjonens rammer.
- [x] Dokumenter hva som er implementert, byggestatus og eventuelle uløste punkter.
- [x] Gi oppdragsgiver en kort sluttrapport med implementerte deler, faktisk
  kjørt byggekommando og resultat, samt eventuelle blokkeringer. Opplys at
  verifisering ikke er utført.
- [x] Flytt planen til `project/process/plans/completed/` når implementasjonsstegene
  er fullført og bygget lykkes uten advarsler.

Ikke kjør verifikatoren, sporingsbygg eller andre UML-til-kode-sammenligninger.
Leveransen skal opplyse at verifisering ikke er utført, og skal ikke påstå
verifisert samsvar.

## Fremdriftsnotater

### 2026-10-13

- Steg 1 fullført: AGENTS.md, specs.md, adr.md, mappingen, relevante eksempler
  og alle åtte inputdiagrammer er lest direkte. Ingen nye inputdiagrammer funnet.
- Steg 2: `project/process/specs/design.md` beskriver alle klasser, attributter,
  metoder, enum-verdier, relasjoner, tilstandsoverganger og scenariomeldinger.
  Dokumentet skiller tegnede krav fra uavklarte forslag.
- Namespace/mappe er `ogl`. De tre entydige status-enumene er implementert i
  `project/impl/include/ogl/`. Category avventer avklaring av `...`.
- Status-headerne er kompilert samlet med
  `c++ -std=c++20 -Wall -Wextra -I project/impl/include -x c++ -fsyntax-only -`
  (include-direktivene gitt på stdin). Resultat: exitkode 0, ingen advarsler.
  Fullt CMake-bygg er ikke utført; klasser og programmer avventer avklaringene.
- Avklaringsspørsmål er sendt om klassesignaturer/oppførsel, tilstandsrepresentasjon
  og sekvensdeltakere/kall. Detaljer og ytterligere avhengige uklarheter er samlet
  i design.md; ingen foreslåtte nye funksjonskrav er implementert.
- Neste steg: innarbeid svarene i design.md, ferdigstill klassene, tilstandsmaskinene,
  CMake, demo og scenarioer; kjør deretter ordinært bygg.
- Planen er fortsatt aktiv og skal ikke flyttes til completed før hele
  implementasjonen er ferdig og bygger uten advarsler.
- Verifikasjon, sporingsbygg og genererte rapporter/sammenligningsdiagrammer er
  ikke kjørt eller lest. Eksisterende slettinger i diagrams/output er ikke endret.

### 2026-10-13 – implementasjon fullført

- Brukeren avklarte: «Du kan fikse logiske brister». Konkrete beslutninger er
  dokumentert i `project/process/specs/design.md`, under «Avklaringer etter
  brukerens klarsignal». De erstatter de tidligere uavklarte forslagene.
- Implementert User, Game, Comment, Review, Repository og Request, enum-typer,
  UUID-alias, relasjoner og fungerende metodekropper i namespace ogl.
- Alle tre tilstandsmaskiner har State/Event, starttilstand, handle og
  overgangstabell med fem eksplisitte felt per rad. User sin tidsavslutning
  representeres av en manuelt tilført hendelse og terminaltilstanden Deleted.
- Implementert demo og scenario_nedlastning, scenario_publish, scenario_request
  og scenario_review. Request følger Accepted → Completed. UI/personer er
  eksterne aktører, og UI-søk/faktisk filnedlasting er utenfor domenekoden.
- CMake setter C++20, eksport av compile_commands.json, offentlig include-mappe,
  statisk bibliotek og -Wall -Wextra for alle seks byggetargets.
- Kjørt `cmake -S project/impl -B project/build`: exitkode 0.
- Kjørt `cmake --build project/build`: exitkode 0. Bibliotek, demo og alle fire
  scenarioer bygget uten kompilatoradvarsler med GNU C++ 14.2.0.
- Ingen videre avklaringer blokkerer implementasjonen etter klarsignalet.
  Begrensninger: repository-validering sjekker kun URL-syntaks lokalt; ID-er
  genereres deterministisk per type og er ikke globale UUID-er; User opprettes
  med shared_ptr, og applikasjonen må eie spillene som biblioteket refererer til.
- Ingen tester, scenariokjøring, tracing eller UML-verifikasjon er utført.
  Inputdiagrammer og eksisterende genererte resultater er ikke endret.
- Planen flyttes til `project/process/plans/completed/plan.md` etter vellykket bygg.
  Uavhengig verifikasjon gjenstår hos oppdragsgiver; ingen samsvarsscore påstås.

## Startbeskjed til en ny AI

> Følg `project/process/plans/plan.md` og gjennomfør hele implementasjonen.
> Les de angitte spesifikasjonene og inputdiagrammene først, skriv koden og bygg
> den med CMake. Oppdater fremdriften i planen underveis. Ikke kjør verifisering.
