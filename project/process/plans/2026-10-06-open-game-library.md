# Implementasjonsplan: UML til C++ — Open Game Library

Dato: 6. oktober 2026.
Status: **Kjørbar mock-up implementert og kontrollert. Gjenværende krav:
100 % alignment mot alle inputmodeller. Planen beholdes her fordi dette
kravet ikke er oppfylt.**

Sluttrapport: [implementasjon og kontroller](../2026-10-06-implementasjonsrapport.md).

Resultat fra første kontroll: [fase 1-gjennomgang](../research/2026-10-06-input-kontroll.md).

Grunnlag: [spesifikasjonen](../specs/2026-10-06-open-game-library.md),
diagrammene i `project/diagrams/input/`, `AGENTS.md` og
`tools/umlverify/docs/UML-CPP-MAPPING.md`.

## Mål og arbeidsregler

Les UML-diagrammene i input og lag C++20-implementasjonen som spesifisert
i specen. Ingen grafisk løsning eller ekstra produktfunksjonalitet.
Bruk CMake, et statisk bibliotek, en liten demo og et scenarioprogram.

**AI-en skal aldri endre input-diagrammene, inkludert filnavn og layout.**
Ved mangler eller motstrid beskrives problemet og forslag til løsning i
tekst. Brukeren/gruppen avgjør og utfører eventuelle modellendringer.
Deretter oppdateres specen før den berørte implementasjonen fortsetter.
Forslagene i `models/codex-forslag/` er ikke fasit.

Brukeren har 6. oktober 2026 instruert AI-en til å gjennomføre uten flere
spørsmål eller godkjenning mellom fasene. Kontrollpunktene er derfor
rapportering og egenkontroll, ikke stoppunkter. AI-en velger minimale,
deterministiske tolkninger der detaljer mangler, og dokumenterer dem i
specen og en ADR før implementasjon. Konflikter mellom diagrammer skal
oppgis som avvik; de skal ikke skjules eller gi en uriktig påstand om 100 %.
Input-diagrammene forblir urørt. Avkryssing bygger på faktisk utført arbeid.

## Fase 1 — Avklar og kontroller input

- [x] Les de tre gjeldende input-diagrammene og specen på nytt.
- [x] Gå gjennom A1–A8 og dokumenter valgte tolkninger: filnavn, Date,
      synlighet, relasjoner, User-maskinen, gjenåpning/tid, sekvensnavn og levetid.
- [x] Behold input uendret og registrer nødvendige modellendringer som avvik.
- [x] Dokumenter større designbeslutninger som ADR-er under `project/process/`.
- [x] Oppdater specens klasser, mapping, overganger og meldingsrekkefølge
      etter de avklarte modellene. Ingen uløste antakelser skal skjules.
- [ ] Kjør `.venv/bin/python tools/verify.py project` og kontroller at alle
      tre diagramtypene oppdages og leses. Manglende implementasjon er
      forventet her; ukjent filnavn eller ignorerte designelementer må avklares.

**Kontrollpunkt 1:** Kontroller at tolkningene er dokumentert, og fortsett
til kodegenerering. Uavklarte modellkonflikter blir rapporterte begrensninger.

## Fase 2 — CMake, klasser og relasjoner

- [x] Opprett `project/impl/CMakeLists.txt` med C++20, compile commands,
      statisk bibliotek, public include-mappe og `-Wall -Wextra`.
- [x] Bruk én namespace og én header per klasse i `include/<name>/`.
      Bruk snake_case-filnavn og source-filer i `src/`.
- [ ] Implementer alle avklarte klasser, attributter, metoder, synligheter
      og arv nøyaktig etter klassediagrammet.
- [ ] Implementer relasjoner med riktig rolle, retning, multiplisitet og
      eierskap. Unngå ekstra medlemmer, getters, hjelpetyper og skjult lagring.
- [ ] Gi metodene minimal mock-oppførsel etter specen; sikre avklart levetid
      for objekter som assosiasjonene peker på.
- [x] Lag en liten `src/demo.cpp` som bruker klassene med faste data.
- [x] Bygg og kjør demoen. Kjør verifikatoren etter endringer og les rapportene.
      Klasseavvik rettes i koden. Uimplementert state/scenario registreres
      som gjenstående arbeid for fase 3–4, ikke som bestått kontroll.

**Kontrollpunkt 2:** Kontroller CMake, klasser, eierskap og klassekontrollen.
Eventuelle gjenstående avvik skal forklares konkret før neste fase.

## Fase 3 — Tilstandsmaskin for User

- [x] Definer nested State og Event etter de avklarte tilstandsnavnene og hendelsene.
- [x] Sett riktig starttilstand på state-medlemmet.
- [x] Implementer tegnede guards og actions; ikke innfør nye regler.
- [x] Definer Transition og `static constexpr Transition transitions[]`
      etter guard/action-deklarasjonene. Hver rad har alle fem felt.
- [x] Implementer `handle(Event)` i source-filen. Hendelser uten gyldig
      overgang ignoreres slik mappingen beskriver.
- [ ] Kontroller starttilstand, lovlige overganger, avviste hendelser og
      begge utfall av relevante guards uten å legge til ekstra UML-medlemmer.
- [ ] Bygg og kjør verifikatoren. Både klasse- og state-rapporten skal vise
      100 %; nye state-relaterte medlemmer må allerede være tegnet i klassen.

**Kontrollpunkt 3:** Kontroller overgangstabellen, guard-oppførselen
og resultatene fra klasse- og state-kontrollene, og fortsett.

## Fase 4 — Sekvensens scenarioprogram

- [x] Opprett `scenarios/user.cpp` og CMake-target
      `scenario_user`, forutsatt filnavnet avklart i fase 1.
- [x] La `main()` opprette nødvendige objekter med faste data og kalle `scenario()`.
- [x] La `scenario()` utføre bare aktørens tegnede kall.
- [x] Sørg for at mellomklassekall kommer fra riktig klasse, til riktig
      metode og i tegnet rekkefølge. Unngå ekstra kall, også getters.
- [x] Behandle konstruktører og returer etter mappingens regler.
- [x] Kjør scenarioet deterministisk uten klokke, tilfeldigheter, input eller tråder.
- [ ] Kjør verifikatoren, les sekvensrapporten og rett kodeavvik.
      Kontroller også at klasse- og state-rapportene fortsatt er 100 %.

**Kontrollpunkt 4:** Kontroller scenarioets kode, faktiske
kallrekkefølge og alle tre verifikasjonsrapportene, og fortsett.

## Fase 5 — Samlet verifikasjon og opprydding

- [x] Bygg fra en ren byggemappe og kontroller at ingen compiler-warnings oppstår.
- [x] Kjør demo og scenario, samt relevante kontroller av returverdier,
      objektlevetid og tilstandsoppførsel. Ikke likestill strukturell alignment
      med full atferdsverifikasjon.
- [x] Kjør `.venv/bin/python tools/verify.py project` etter siste kodeendring.
- [ ] Les hver rapport: klasse, User-state og anmeldelsessekvens.
      Alle skal finnes og vise 100 %, uten uavklarte Changed/Missing/Extra
      eller hopp over nødvendige designelementer.
- [ ] Kontroller at input-diagrammene ikke er endret av AI-en, og at
      implementasjonen ikke inneholder funksjoner utenfor specen.
- [x] Rett eventuelle kodefeil og kjør berørte kontroller igjen.
      Modellkonflikter tas tilbake til brukeren; ikke endre fasiten eller rapportene.

**Kontrollpunkt 5:** Samle kommandoer, rapporter og begrensninger.
Skill en ferdig kjørbar mock-up fra oppnådd 100 % modellalignment.

## Fase 6 — Leveranse og gjennomgang

- [x] Skriv en kort sluttrapport under `project/process/` om samsvar,
      lesbarhet, dokumentasjon, struktur, utførte kontroller og begrensninger.
- [x] Presenter hvilke filer som er laget/endret, og hvordan demo og scenario kjøres.
- [ ] La brukeren gjennomgå kode og Git-diff før eventuell commit/push.
- [x] Registrer gjennomført leveranse og eventuelle gjenværende modellavvik.
- [ ] Flytt denne planen til `plans/completed/` først når alle punktene er
      utført og verifikatoren rapporterer 100 % på alle modellene.
      Oppdater relative dokumentlenker ved flytting.

**Kontrollpunkt 6:** Presenter leveransen uten å vente på ny godkjenning.
Push utføres av brukeren med mindre det gis en egen instruksjon om noe annet.

## Faktisk status etter gjennomføring

Implementasjonen og bygg/kjøring er levert etter den automatiske tolkningen
i ADR 0001. Punkter med krav om nøyaktig samsvar eller 100 % er fortsatt åpne.
Ingen godkjenning fra brukeren eller runtime-kontroll av private state/guard-
verdier er påstått. Bevar planen utenfor completed og bruk sluttrapporten
som oversikt over konkrete modellavvik.
