# Spesifikasjon: UML til C++ — Open Game Library

Dato: 6. oktober 2026.
Status: **Automatisk implementasjon med dokumenterte antakelser. Input har
modellkonflikter som skal rapporteres, ikke endres av AI-en.**

Gjeldende tolkninger: [ADR 0001](../adr/0001-automatisk-uml-tolkning.md).
Denne ADR-en er grunnlag for gjennomføring uten flere spørsmål; den er ikke
en påstand om at input-modellene er endret eller samordnet.

## 1. Oppdrag og fasit

Lag C++-kode som gjengir brukerens tre diagrammer. Ingen grafisk løsning
eller ekstra produktfunksjonalitet skal legges til. Dette er en mock-up,
ikke et krav om virkelig nedlasting, Git-integrasjon eller e-postutsending.

Følgende filer er gjeldende input:

- `project/diagrams/input/class.drawio`
- `project/diagrams/input/state-user.drawio`
- `project/diagrams/input/sequence-user.drawio`

`models/codex-forslag/` og modellene til andre gruppemedlemmer er ikke fasit
for dette arbeidet. Tidligere AI-forslag til roller, score, requests,
komposisjoner og valideringsregler er ikke vedtatt og skal ikke innføres.

**Bindende krav: AI-en skal ikke endre input-diagrammene.** Dette gjelder
innhold, navn på modellelementer, relasjoner, layout og filnavn. Diagrammene
er skrivebeskyttet fasit for AI-arbeidet; bare brukeren/gruppen utfører
endringer i dem. AI-en kan lese og analysere diagrammene og beskrive konkrete
endringsforslag i tekst. Den skal aldri endre diagrammene for å få koden
til å passe eller for å oppnå en bedre verifikasjonsrapport.

Brukeren har 6. oktober 2026 bedt om gjennomføring uten flere spørsmål.
Ved feil, mangler eller motstrid skal AI-en derfor dokumentere problemet og
velge en minimal tolkning i en ADR før implementasjon. Tolkningen endrer
ikke diagrammet og skal rapporteres som avvik når den ikke samsvarer med
fasiten. Brukeren kan senere rette modellen; da oppdateres spec og kode.
Verifikatorens genererte filer i `project/diagrams/output/` er avledede
kontrollresultater og erstatter aldri input som fasit.

Uklarheter skal dokumenteres og avklares;
koden skal ikke løse motstridende modeller ved å legge til egne designelementer.
Denne spesifikasjonen beskriver nåværende modeller og peker på nødvendige
avklaringer. Den godkjenner ikke modellendringer gjennom teksten alene.

## 2. Teknisk ramme

- C++20 og CMake, med `CMAKE_EXPORT_COMPILE_COMMANDS ON`.
- Én header per klasse i `project/impl/include/<name>/`, snake_case-filnavn.
- Én source per klasse med kode i `project/impl/src/`.
- Én felles namespace. Forslag til teknisk navn: `game_library`.
- Statisk bibliotek, public include-mappe, `-Wall -Wextra`, bygg uten advarsler.
- Et lite `src/demo.cpp` og ett deterministisk program per sekvensdiagram.
  Dette er kjørbare kontrollprogrammer, ikke et brukergrensesnitt.
- Ingen ekstra klasser, metoder, attributter eller relasjoner utover designet.
  Konstruktører, destruktører og operatorer er tillatt etter repoets regler.
- Følg `tools/umlverify/docs/UML-CPP-MAPPING.md`.

## 3. Klassene som er tegnet

Tabellene gjengir attributter og metoder lest direkte fra klassediagrammet.
`+` blir public og `-` blir private. Alle er vanlige klasser; ingen statiske
eller abstrakte medlemmer er tegnet. `Developer` arver public fra `User`.

`int` blir `int`, `void` blir `void`, og `string`/`String` blir `std::string`.
Klassenavn beholdes, inkludert `Review_Comment` og `Repo_Comment`.
Klasseparametere kan være `const T&` etter mappingen; ikke endre tegnet
semantikk eller returtype uten avklaring. `Date` er foreløpig udefinert.

### User

| UML-medlem | C++-mapping |
|---|---|
| `- user_id: int` | private: `int user_id` |
| `- user_name: string` | private: `std::string user_name` |
| `- email: string` | private: `std::string email` |

### Developer

| UML-medlem | C++-mapping |
|---|---|
| `+ webpage_url: String` | public: `std::string webpage_url` |
| `+ bio: String` | public: `std::string bio` |
| `+ create_repository(): void` | public metode, `void create_repository()` |
| `+ join_repository(Repository repository): void` | public metode, `void join_repository(Repository repository)` |

### Game

| UML-medlem | C++-mapping |
|---|---|
| `- game_id: int` | private: `int game_id` |
| `- title: String` | private: `std::string title` |
| `- version: String` | private: `std::string version` |
| `- release_date: Date` | Synlighet beholdes; `Date` må avklares (A2). |
| `+ add_review(User author, String content): Review` | public metode, `Review add_review(User author, std::string content)` |
| `+ download(): Game` | public metode, `Game download()` |

### Review

| UML-medlem | C++-mapping |
|---|---|
| `+ date: Date` | Synlighet beholdes; `Date` må avklares (A2). |
| `+ content: String` | public: `std::string content` |
| `+ add_comment(User author, String content): Review_Comment` | public metode, `Review_Comment add_comment(User author, std::string content)` |
| `+ remove_comment(Review_Comment comment): void` | public metode, `void remove_comment(Review_Comment comment)` |

### Repository

| UML-medlem | C++-mapping |
|---|---|
| `+ repository_id: int` | public: `int repository_id` |
| `+ url: String` | public: `std::string url` |
| `+ add_comment(Developer author, String content): Repo_Comment` | public metode, `Repo_Comment add_comment(Developer author, std::string content)` |
| `+ remove_comment(Repo_Comment comment): void` | public metode, `void remove_comment(Repo_Comment comment)` |
| `download(): Repository` | Tegnet, men leseren hopper over metoden fordi synlighet mangler. Må avklares (A3). |

### Review_Comment

| UML-medlem | C++-mapping |
|---|---|
| `- content: String` | private: `std::string content` |
| `+ edit_content(String new_content): void` | public metode, `void edit_content(std::string new_content)` |

### Repo_Comment

| UML-medlem | C++-mapping |
|---|---|
| `- content: String` | private: `std::string content` |
| `+ edit_content(String new_content): void` | public metode, `void edit_content(std::string new_content)` |

Ingen implementasjonsdetaljer som score, autorisasjonssjekk, bibliotek,
feature requests eller database er spesifisert av disse klassene.
Metoder uten tegnet detaljatferd får bare minimal mock-oppførsel som
oppfyller signatur og avklart design. Ikke legg til sideeffekter eller
andre klassekall som sekvensen ikke viser. Konkret oppførsel for opprettelse,
returverdier og lagring av assosierte objekter avklares under A8.

## 4. Relasjoner og mapping

Dette er **diagramleserens tolkning** av linjene, ikke en godkjenning av
navigeringsretning eller manglende rollenavn. Urettede forbindelser får
retning fra source/target i draw.io-filen. Flere forbindelser har dupliserte
multiplisitetsmerker; se A4 før denne tabellen brukes som implementasjonsfasit.

| Fra | Relasjon | Til | Fjern multiplisitet lest | C++-konstruksjon etter avklaring |
|---|---|---|---|---|
| Developer | Arv | User | — | `class Developer : public User` |
| Developer | Assosiasjon | Repo_Comment | Ikke angitt | Ikke-eiende peker/referanse; antall og rollenavn avklares |
| Game | Assosiasjon | Review | `0..*` | `std::vector<Review*>`, rollenavn avklares |
| Repository | Assosiasjon | Developer | `1..*` | `std::vector<Developer*>`; minst én må ivaretas i oppsett/atferd |
| User | Assosiasjon | Review | `0..*` | `std::vector<Review*>`, rollenavn avklares |
| Repository | Assosiasjon | Game | Ikke angitt | Ikke-eiende peker/referanse; antall og rollenavn avklares |
| Review | Assosiasjon | Review_Comment | `0..*` | `std::vector<Review_Comment*>`, rollenavn avklares |
| User | Assosiasjon | Review_Comment | `0..*` | `std::vector<Review_Comment*>`, rollenavn avklares |
| Repo_Comment | Assosiasjon | Repository | Ikke angitt | Ikke-eiende peker/referanse; antall og rollenavn avklares |

Det er ingen tegnede komposisjoner eller aggregasjoner i denne inputen.
Assosiasjoner skal derfor ikke automatisk bli `unique_ptr`, by-value-medlemmer
eller `shared_ptr`. En relasjon representerer ett medlem og skal ikke
føres opp på nytt som et vanlig attributt. Parametertyper kan gi implisitte
avhengigheter etter verifierens regler.

## 5. User sin tilstandsmaskin

Tegnede tilstander blir nested `enum class State`:

| UML | C++-identifikator |
|---|---|
| Awaiting email verification | `AwaitingEmailVerification` |
| Active | `Active` |
| Suspended | `Suspended` |
| Closed | `Closed` |

Starttilstanden er `AwaitingEmailVerification`.
Hver avklart overgang skal bli én rad med alle fem felt i
`static constexpr Transition transitions[]`: from, event, to, guard, action.
Ingen actions er tegnet på de fem entydige overgangene nedenfor.

| Fra | Hendelse | Til | Guard/action |
|---|---|---|---|
| Awaiting email verification | Email confirmation → `EmailConfirmation` | Active | `nullptr`, `nullptr` |
| Active | Close account → `CloseAccount` | Closed | `nullptr`, `nullptr` |
| Active | Suspend account → `SuspendAccount` | Suspended | `nullptr`, `nullptr` |
| Suspended | Close account → `CloseAccount` | Closed | `nullptr`, `nullptr` |
| Suspended | Reactivate account → `ReactivateAccount` | Active | `nullptr`, `nullptr` |
| Closed | Gjenåpning er antydet av løs tekst | Active | «no active suspension» er antydet; pilen mangler lesbar etikett |
| Closed | After(30 days) | Sluttsymbol | Støttes ikke direkte av C++-mappingen; må avklares |

`Event` inneholder de avklarte hendelsene. `handle(Event)` skal gå gjennom
transition-tabellen og ignorere hendelser uten en gyldig rad. Startverdien
skal ligge i initialiseringen av state-medlemmet.

**Modellkonflikt:** Klassediagrammets User har ikke `state`, `handle` eller
metoden for gjenåpningsguarden. Disse er implementert etter tilstandsmaskinen
og ADR 0001, og rapporteres som ekstra klasseelementer. Input er beholdt.

## 6. Sekvens: sende inn anmeldelse

Sekvensens deltakere er aktøren `User`, `game_page`, `game: Game` og
`review: Game_Review`. User-aktøren representerer den eksterne brukeren;
den er ikke automatisk et kall fra C++-klassen User.

Tegnet hensikt, i samtalerekkefølge:

1. User → game_page: «Send inn review».
2. game_page → Game: `add_review(author, content)`.
3. Game → Game_Review: `«create» (author, content)`.
4. Game_Review → Game: retur med anmeldelsen.
5. Game → game_page: retur med anmeldelsen.

Trinn 1–2 skal etter avklaring bli vanlige metodekall. Trinn 3 er en
konstruktør og sammenlignes ikke i runtime-tracen. Returer er dokumentasjon.
Det finnes også en løs stiplet linje som leseren tolker som en tom retur
User → User; den uttrykker ikke et implementasjonskrav.

Merk at noen piler er plassert slik at readerens y-sortering gir en annen
returrekkefølge. Det påvirker ikke sammenlignede kall, men bør ryddes opp
for at tegningen skal vise ønsket samtaleforløp tydelig.

**Modellkonflikter:** `game_page` og `Game_Review` finnes ikke som klasser.
Klassediagrammet har `Review`. «Send inn review» er ikke en navngitt
klassemetode. Dette er tolket under A7 i ADR 0001. Controlleren og navneforskjellene
rapporteres som avvik mellom implementasjonen og input.
At grafisk grensesnitt er utenfor omfanget utelukker ikke en ren
controller-klasse, men en slik klasse må i så fall tegnes av brukeren.

Når sekvensen er avklart, opprettes `scenarios/user.cpp`:
`main()` lager objekter med deterministiske data og kaller `scenario()`.
`scenario()` gjør bare aktørens kall. Alle mellomklassekall skal følge
sekvensen i riktig rekkefølge, uten ekstra getters eller valideringskall.
Ingen klokke, tilfeldigheter, input eller tråder brukes.

## 7. Avklaringer før implementasjon

| ID | Problem | Hva brukeren må avgjøre eller rette |
|---|---|---|
| A1 | Løst: brukeren har gitt inputfilene støttede navn. | Gjeldende navn er `class.drawio`, `sequence-user.drawio` og `state-user.drawio`. Scenarioet er `scenarios/user.cpp` med target `scenario_user`. |
| A2 | `Date` brukes, men er verken tegnet eller en definert C++-type her. | Velg en konkret type/mapping og dokumenter den. Ikke legg til en egen Date-klasse eller bytt til string uten beslutning. |
| A3 | Repository.download mangler synlighet. | Sett eksplisitt `+`, `-` eller `#`; behold avklart returtype. |
| A4 | Relasjoner har ingen rollenavn, uklar navigering og enkelte motstridende antallsmerker. | Tegn retning, rollenavn og entydig multiplisitet; avklar eierskap og objektenes levetid. |
| A5 | User sin klasse og tilstandsmaskin stemmer ikke overens. | Tegn state, handle og eventuell guard/action med nødvendig datagrunnlag i klassen. |
| A6 | Reopen-teksten er løs; tidsmerket overgang til sluttsymbol støttes ikke. | Fest event/guard til pilen. Avklar hvordan 30-dagershendelsen representeres deterministisk, og hva «ingen aktiv suspensjon» krever av data. |
| A7 | Review/Game_Review, game_page og aktørmelding er ikke samordnet med klassene. | Velg felles klassenavn. Enten tegn controller og metode eller la aktøren kalle Game direkte; tilpass sekvensen tilsvarende. |
| A8 | Assosiasjonene sier ikke hvem som eier nye anmeldelser/kommentarer, og metodene returnerer objekter. | Avklar hvordan opprettede objekter lever og refereres til, uten dangling pointers, skjult lagring eller utilsiktede kopier. Behold bare oppførsel som designet faktisk krever. |

Løsningene i tabellen er ikke vedtatte modellendringer. AI-en kan etter
brukerens nye instruksjon velge dokumenterte implementasjonstolkninger uten
å spørre. Behovet for å dokumentere forskjellene
følger av `AGENTS.md`, reglene «Implement exactly what is drawn» og «Use the
design's names and types», og mappingen som krever state/handle/guard i
klassediagrammet. De valgte implementasjonstolkningene står i ADR 0001. Senere
modellvalg fra brukeren skal oppdatere spesifikasjonen og koden.

## 8. Akseptansekriterier

1. Modeller og denne specen er avklart uten skjulte antakelser.
2. CMake bygger C++20-bibliotek, demo og scenario uten warnings.
3. Alle tegnede klasser, attributter, metoder og relasjoner finnes nøyaktig
   som avtalt, og ingen ekstra designelementer er lagt til.
4. Tilstandstabellen samsvarer med alle avklarte states/events/overganger,
   starttilstand, guards og actions.
5. Scenarioet gir den tegnede kallrekkefølgen deterministisk.
6. `.venv/bin/python tools/verify.py project` finner **alle tre** inputene,
   og samtlige klasse-, state- og sekvensrapporter viser 100 % alignment.
   Manglende rapporter og uleselige/ignorerte designelementer er ikke suksess.
7. Kjør demo/scenario og relevante atferdskontroller. Structural alignment
   alene dokumenterer ikke returverdier, levetid eller guard-logikk.
8. Lever en kort rapport om lesbarhet, struktur, dokumentasjon, kontrollene
   som er kjørt og gjenværende avvik. Brukeren vurderer resultatet før push.

## 9. Nåværende kontrollstatus

De tre filene er lest direkte med repoets diagramlesere uten endringer.
Klasser: 7. Relasjoner lest: 9, inkludert arv. Repository.download ble
hoppet over; User-maskinen har uleselig gjenåpningsetikett og en ikke-støttet
tidsovergang. Sekvensleseren ignorerer create-meldingen som dokumentert.

C++-implementasjonen finnes i `project/impl/`. Full alignment er ikke
oppnådd: separat kontroll av byte-identiske inputkopier gir 55,9 % for
klasse, 70,0 % for state og 57,1 % for sekvens. Filnavnene er senere rettet av brukeren. Standardkommandoen kan nå
kontrollere alle tre modeller; de øvrige modellavvikene består.
Utført implementasjon og verifikasjon er beskrevet i
[sluttrapporten](../2026-10-06-implementasjonsrapport.md).
