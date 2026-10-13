# Implementasjonsgrunnlag fra inputdiagrammene

Lest direkte fra XML i `project/diagrams/input/` 2026-10-13. Ingen genererte
diagrammer, rapporter, verifikator eller sporingsbygg er brukt. Dokumentet skiller
tegnede krav fra forslag som fortsatt må avklares. Namespace: `ogl`.

## Felles C++-mapping

- Én header per klasse under `include/ogl/`, metodekropper i `src/`.
- Alle tegnede attributter er private; alle tegnede metoder er offentlige.
- `string` blir `std::string`; `int` og `bool` beholdes; `T[]` blir
  `std::vector<T>` med eierskap bestemt av relasjonen.
- `uuid` er ikke definert. Forslag: `using uuid = std::string`, uten generator.
- Ingen tegnede metoder har eksplisitt returtype. Returtyper som trengs for å
  hente opprettede objekter eller vise forespørsler må avklares før implementasjon.
- Relasjonsmedlemmer deklareres én gang, selv om de også er oppført i klasseboksen.
- Konstruktører/destruktører kan brukes til initialisering og eierskap.
- Ingen statiske, beskyttede eller kursiverte metoder er tegnet.

## Klassebokser

### User

| Tegnet element | C++-mapping / uavklart punkt |
|---|---|
| `- id: uuid` | privat `uuid id_`; uuid uavklart |
| `- username: string` | privat `std::string username_` |
| `- game_library: Game[]` | privat vektor; eierskapet er ikke angitt av en separat pil for denne rollen |
| `- marked_for_delete: bool` | privat `bool marked_for_delete_` |
| `- user_status: User_status` | privat enum-verdi; boksen heter `User_Status` |
| `+ add_game_to_library(game: Game)` | offentlig metode med Game-parameter; returtype mangler |
| `+ create_new_game(name: string, repo_url: string)` | offentlig metode med to strenger; returtype og plassering av nytt spill må avklares |

### Game

| Tegnet element | C++-mapping / uavklart punkt |
|---|---|
| `- id: uuid` | privat `uuid id_` |
| `- name: string` | privat `std::string name_` |
| `- creator: User` | privat `std::shared_ptr<User> creator_`, hul diamant ved Game |
| `- game_status: Game_status` | privat `Game_status game_status_` |
| `- game_repository: Repository` | privat Repository ved verdi eller unique_ptr, fylt diamant ved Game, multiplisitet 1 |
| `- categories: Category[]` | privat `std::vector<Category> categories_` |
| `- reviews: Review[]` | privat `std::vector<std::shared_ptr<Review>> reviews_`, hul diamant ved Game, multiplisitet * |
| `+ add_review(user: User, title: string, message: string)` | offentlig metode; må beholde tre parametere med mindre bruker avklarer annet |
| `+ add_category(category: Category)` | offentlig metode med Category-verdi |

### Comment {abstract}

| Tegnet element | C++-mapping / uavklart punkt |
|---|---|
| `- id: uuid` | privat `uuid id_` |
| `- user: User` | privat `std::shared_ptr<User> user_`, hul diamant ved Comment, multiplisitet 1 ved User |
| `- title: string` | privat `std::string title_` |
| `- description: string` | privat `std::string description_` |
| `+ update(title: string, description: string)` | offentlig metode; oppdaterer de to tekstfeltene |

Ingen metode er merket abstrakt. Forslag til avklaring: ren virtuell destruktør
med definisjon i `.cpp`, slik at klassen blir abstrakt uten en ekstra domenemetode.

### Review

| Tegnet element | C++-mapping / uavklart punkt |
|---|---|
| `- score: int` | privat `int score_` |
| `+ update_score()` | offentlig parameterløs metode; poengkilde/beregning mangler |

### Repository

| Tegnet element | C++-mapping / uavklart punkt |
|---|---|
| `- id: uuid` | privat `uuid id_` |
| `- repository_url: string` | privat `std::string repository_url_` |
| `- feature_requests: Request[]` | privat `std::vector<std::shared_ptr<Request>> feature_requests_`, hul diamant ved Repository, multiplisitet * |
| `+ add_feature_request(user: User, title: string, description: string)` | offentlig metode; konstruerer Request og legger den i feature_requests |
| `+ show_requests()` | offentlig metode; returtype/visningsformat mangler |

### Request

| Tegnet element | C++-mapping / uavklart punkt |
|---|---|
| `- votes: int` | privat `int votes_` |
| `- status: Request_Status` | privat `Request_Status status_` |
| `+ upvote()` | offentlig metode; øker votes med én |
| `+ downvote()` | offentlig metode; reduserer votes med én |
| `+ update_status(status: Request_Status)` | offentlig metode; overganger må følge state-request |

### Enum-typer

| UML-navn | Navngitte verdier i rekkefølge | C++ |
|---|---|---|
| Category | Action, Adventure, Puzzle, Simulation, Shooter, `...` | `enum class Category`; `...` er ikke en C++-enumerator og krever avklaring |
| Request_Status | Pending, Rejected, Accepted, Completed | `enum class Request_Status` |
| User_Status | Active, Suspended, Pending | `enum class User_Status` |
| Game_status | Draft, Published, Archived | `enum class Game_status` |

## Relasjoner og faktisk pilgeometri

| XML-forbindelse | Tegnet eierskap og C++-mapping |
|---|---|
| Request → Repository, `endArrow=diamondThin;endFill=0` | Repository aggregerer * Request; `feature_requests_` som vektor av shared_ptr |
| Review → Game, `endArrow=diamondThin;endFill=0` | Game aggregerer * Review; `reviews_` som vektor av shared_ptr |
| Game → Repository, `startArrow=diamondThin;startFill=1` | Game komponerer 1 Repository; `game_repository_` ved verdi/unique_ptr |
| User → Comment, `endArrow=diamondThin;endFill=0` | Comment aggregerer 1 User; `user_` som shared_ptr; * ved Comment er nærende |
| User → Game, `endArrow=diamondThin;endFill=0` | Game aggregerer 1 User; `creator_` som shared_ptr; * ved Game er nærende |
| Comment → Review | XML mangler eksplisitt arv-pil (`endArrow=block;endFill=0`); ikke entydig arv etter mappingen |
| Comment → Request | Samme problem; forslag: begge er offentlige underklasser av Comment |

`User.game_library` har ingen egen relasjonspil. `std::vector<Game>` ville etter
mappingen være komposisjon; en vektor av shared_ptr ville være aggregering. Dette
er et eierskapsvalg, og ikke avgjort ved å snu creator-pilen.

## Tilstandsmaskiner

Felles påkrevd mapping: nestet `enum class State`, nestet `enum class Event`,
privat tilstand med startinitialisering, offentlig `void handle(Event)`, privat
`Transition` og `static constexpr Transition transitions[]`. Hver rad har
`{from, event, to, guard, action}`, med nullptr der guard/handling mangler.
`handle` går gjennom tabellen, sjekker guard, kjører handling, setter tilstand,
og ignorerer en hendelse uten gyldig rad.

Klasseboksene mangler `state: State`, `handle(Event)` og guard-metodene.
Avklaring kreves om tilleggene og forholdet mellom State og eksisterende status.

### Game

- Startpil: start → Draft, merket `createGame`; blir startinitialisering.
- Tilstander i XML-rekkefølge: Draft, Validation, Published, Draft, Archived.
  Draft forekommer i to bokser; forslag: samme tilstand ved feil og ved start.
- Draft → Validation: ingen etikett/hendelse.
- Validation → Published: etikett `(valid repo)`.
- Validation → Draft: etikett `(invalid repo)`.
- Published → Archived: `archiveGame`.
- Archived → sluttsymbol: ingen etikett.
- Ingen eksplisitte `/ action`-etiketter.

`valid repo`/`invalid repo` er ikke skrevet som navngitte `[guard]` og det finnes
ingen valideringsmetode, valideringsresultat eller definert valideringsregel.
Validation mangler i Game_status. Ingen tabellrad for disse overgangene velges
før hendelser, guards og statusrepresentasjon er avklart.

### Request

- Start: Pending.
- Tilstander: Pending, Rejected, Accepted, Completed.
- Pending → Accepted: umerket pil.
- Pending → Rejected: umerket pil.
- Rejected → Pending: `update()`.
- Accepted → Completed: umerket pil.
- Ingen eksplisitte guards eller handlinger; Completed har ingen utgående pil.

XML knytter alle tre tekstetikettene `update()`, `update_status()` og
`update_status()` til Rejected → Pending. De to sistnevnte er geometrisk flyttet
til de horisontale pilene Pending → Accepted og Accepted → Completed. Dette
dokumenteres som forskjell mellom tilkobling og synlig plassering, ikke som tre
hendelser på én pil. Forslag til avklaring: statusargumentet skiller accept,
reject og complete; redigering etter avvisning setter Pending.

### User

- Start: Awaiting email verification.
- Tilstander: Awaiting email verification, Active, Suspended, Closed.
- Awaiting email verification → Active: `Email confirmation`.
- Active → Suspended: `Suspend account`.
- Suspended → Active: `Reactivate account`.
- Active → Closed: `Close account`.
- Suspended → Closed: `Close account`.
- Closed → Active: umerket pil, nærliggende frittstående tekst
  `Reopen account [no active suspension]`.
- Closed → sluttsymbol: `After(30 days)`.
- Ingen eksplisitte handlinger.

Eventnavn kan normaliseres til gyldige C++-identifikatorer. En guard for
`no active suspension` trenger informasjon som eksisterende attributter ikke
entydig bevarer etter Suspended → Closed. Closed mangler i User_Status;
forholdet mellom Pending og Awaiting email verification må avklares.
Mappingen representerer sluttsymbol som terminal tilstand uten ekstra rad,
mens Closed også har en gjenåpningspil: tidsavslutningen trenger derfor en
eksplisitt avklaring. Ingen klokke eller reell venting skal legges i scenarioene.

## Sekvensdiagrammer

Hvert avklart diagram får `scenarios/<navn>.cpp` og `scenario_<navn>` i CMake.
`main()` setter opp objekter; `scenario()` gjør bare aktørkall. Kall mellom
klasser ligger i metodene hos tegnet avsender. Konstruksjoner er ikke vanlige
meldinger; returmeldinger er dokumentasjon. Ingen ekstra getter-kall innføres.

### nedlastning

Deltakere fra venstre: Bruker, Offentlig bibliotek, Spillbibliotek, Spill.

| Rekkefølge / y | Avsender → mottaker | Etikett og art |
|---|---|---|
| 1 / 120 | Bruker → Offentlig bibliotek | `SøkerSpill()`; kall |
| 2 / 150 | Offentlig bibliotek → Bruker | `Vis spill`; stiplet retur med pil ved startenden |
| 3 / 200 | Bruker → Offentlig bibliotek | `LastNed()`; tekst er frittstående ved en umerket pil |
| 4 / 230 | Offentlig bibliotek → Bruker | `Nedlastning vellykket`; stiplet retur |
| 5 / 250 | Offentlig bibliotek → Spillbibliotek | `legg til i spillbibliotek`; frittstående tekst ved pil |
| 6 / 300 | Offentlig bibliotek → Spillbibliotek | umerket pil; frittstående `Text` over pilen |
| 7 / 320 | Bruker → Offentlig bibliotek | `return`; stiplet retur |

Spill mottar ingen pil. Klassene Offentlig bibliotek/Spillbibliotek og metodene
SøkerSpill/LastNed er ikke i klassedesignet. Kall 6 mangler mening. Det kan ikke
implementeres nøyaktige scenariokall uten avklaring.

### publish

Deltakere: Developer (aktørsymbol), Developer Portal, Repository, Game.

1. Developer → Developer Portal: `Publiser spill` (y=160).
2. Developer Portal → Repository: `Validate` (y=180).
3. Repository → Developer Portal: `Valid` (y=200).
4. Developer Portal → Game: `Create Game` (y=280).
5. Game → Developer Portal: `Game Created` (y=310).
6. Developer Portal → Developer: `Publisert` (y=360).

3, 5 og 6 ligner semantisk returmeldinger, men er tegnet heltrukket med pil i
startenden. 4 ligner konstruksjon, men er ikke merket «create». Developer Portal
og Validate er ikke i klassedesignet; User.create_new_game finnes, men har ingen
livslinje her. Det kreves en avklaring av deltakere, kall og retur/konstruksjon.

### request

Deltakere: requester:User, players:User, developer:User, :Repository, :Request.

1. requester:User → Repository: `add_feature_request(title, description)` (107).
2. Repository → Request: `Request(user, title, description)` (167), konstruksjon.
3. Request → Repository: `Request created` (217), stiplet retur.
4. requester:User → Request: `upvote()` (250).
5. players:User → Request: `upvote()` (297).
6. players:User → Request: `downvote()` (361).
7. Øvre alt-gren: developer:User → Request: `update_status(Accepted)` (447),
   så `update_status(Completed)` (517). `Request in development` er en note.
8. Nedre alt-gren: developer:User → Request: `update_status(Rejected)` (607),
   requester:User → Request: `update(title, description)` (657),
   Request → Request: `update_status(Pending)` (677–727),
   Request → requester:User: `Request updated` (767), stiplet retur.

Grenene skal ikke kjøres som én sekvens. Én gren må velges for det deterministiske
programmet. Mappingen støtter ikke alt-fragmenter eller flere objekter av samme
klasse. User mangler metoder som kan sende disse kallene fra en User-metode;
livslinjene må avklares som eksterne aktører eller få eksplisitt grensesnitt.
Klassemetoden add_feature_request har i tillegg et User-argument.

### review

Deltakere: User (aktørsymbol), game_page, game:Game, review:Game_Review.

1. User → game_page: `Send inn review`.
2. game_page → Game: `add_review(author, content)`.
3. Game → Game_Review: `<<create>> (author, content)`, konstruksjon.
4. Game_Review → Game: `review: Game_Review`, stiplet retur.
5. Game → game_page: `review`, stiplet retur.

game_page og Game_Review er ikke klasser i klassedesignet. Forslag til
avklaring: game_page er ekstern aktør/grenseflate og Game_Review betyr Review.
Klassemetoden har tre parametere (user, title, message), ikke to; en konkret
tittel og tekst må gis i scenarioet dersom klassesignaturen skal brukes.

## Uavklarte krav før fullføring

1. uuid-representasjon, returtyper og Category sin ellipsis.
2. Eierskap til User.game_library og hva create_new_game returnerer/lagrer.
3. Arv fra Comment og hvordan abstraktheten uttrykkes.
4. Datakilde for update_score og resultat/visning fra show_requests.
5. State/handle/guards i forhold til klasseboksene og status-enumene.
6. Manglende Game-hendelser og definisjon av repository-validering.
7. User sin suspensjonshistorikk, gjenåpning og tidsavslutning.
8. Request-etikettenes tilkobling og statusbaserte hendelser.
9. Sekvensdeltakernes klasse-/aktørmapping, manglende kall og request-gren.

Det er stilt avklaringsspørsmål i samtalen. Forslagene over er ikke vedtatte krav.
Implementasjonen skal ikke markeres ferdig før avklarte krav er gjennomført og
hele bygget lykkes uten advarsler. Verifikasjon er ikke utført.

## Avklaringer etter brukerens klarsignal

Brukeren svarte «Du kan fikse logiske brister». Punktene nedenfor erstatter de
åpne forslagene over som implementasjonsbeslutninger. Inputdiagrammene endres ikke.

- uuid blir en string-alias. Konstruktører tar imot ID ved oppsett; ved intern
  opprettelse brukes deterministiske, lokalt genererte ID-er uten klokke/tilfeldighet.
- Category får de fem navngitte verdiene; ellipsen betyr ingen konkret ekstra verdi.
- `User_Status` er felles skrivemåte. De tre status-enumene beholdes som tegnet.
- Review og Request arver offentlig fra Comment. Comment får ren virtuell
  destruktør og en virtuell, implementert `void update(string, string)`.
- `Review::update_score(int score)` får den manglende parameteren og setter score.
  Ny review starter på 0, siden add_review ikke tar imot poeng.
- Muterende metoder returnerer void, med unntak av create_new_game,
  add_review og add_feature_request, som returnerer shared_ptr til objektet de
  oppretter. show_requests returnerer en const-referanse til request-samlingen.
- Game.creator og Comment.user følger de hule diamantene: shared_ptr<User>.
  Game eier Repository ved verdi. Review- og Request-samlingene bruker shared_ptr.
- User.game_library blir en vektor av weak_ptr<Game>: biblioteket refererer til
  spill som oppsettet/applikasjonen holder i live. Dette unngår en eierskapssyklus
  mellom Game.creator og User.game_library. add_game_to_library tar shared_ptr<Game>.
  User bruker standardbibliotekets enable_shared_from_this for at create_new_game
  kan gi Game korrekt delt eierskap til den eksisterende skaperen. User-objekter
  opprettes derfor med make_shared i demo og scenarioer.
- Alle tre tilstandsmaskinene får egne State/Event-enumerasjoner, state_, handle
  og femfelts overgangstabell. Eksisterende statusfelt oppdateres av handle.
- Game: Draft --submit--> Validation; Validation --validate [valid_repo]-->
  Published; Validation --validate [invalid_repo]--> Draft; Published
  --archiveGame--> Archived. De to Draft-boksene er samme tilstand.
- Repository får `bool validate() const`: en lokal, deterministisk sjekk av at
  URL-en har en støttet protokoll (https/http/ssh/git) og en ikke-tom vert/stidel,
  uten whitespace. Dette er syntakssjekk, ikke nettverks- eller git-validering.
  Game sine guards bruker denne metoden.
- Request: Pending --accept--> Accepted; Pending --reject--> Rejected;
  Accepted --complete--> Completed; Rejected --update--> Pending.
  update_status oversetter statusargumentet til en hendelse; ugyldige overganger
  ignoreres. Request overstyrer update, oppdaterer Comment-teksten og gjør tegnet
  selvkall update_status(Pending) ved redigering av en avvist request.
- User: tegnede hendelser normaliseres til email_confirmation, suspend_account,
  reactivate_account, close_account og reopen_account. Closed --after_30_days-->
  Deleted representerer pilen til sluttsymbolet som eksplisitt terminal tilstand.
  Hendelsen tilføres uten klokke. Guard no_active_suspension undersøker user_status;
  Closed bevarer forrige Active/Suspended-status, mens state_ og marked_for_delete_
  registrerer lukking. Dermed trengs ingen ekstra historikkvariabel.
- Eksterne UI-deltakere og personer representeres av scenario-funksjonen.
  Game_Review betyr Review. Klassemetodene beholder fullstendige bruker-/tekstparametere.
- nedlastning: UI-søk/nedlasting ligger utenfor de seks klassene. Scenarioet velger
  et spill deterministisk fra objektene i oppsettet og kaller User.add_game_to_library.
  Ingen HTTP-nedlasting eller nye bibliotekklasser innføres. Den umerkede Text-pilen
  får ingen oppdiktet domenemetode.
- publish: aktøren kaller User.create_new_game; konstruksjonen lager Game og
  Repository. User kaller deretter Game.handle(submit), så Game.handle(validate).
  Game kaller Repository.validate via guard og går til Published når URL-en er gyldig.
  Dette konkretiserer «Validate» og «Create Game» med eksisterende domeneobjekter.
- request: de tre User-livslinjene behandles som eksterne brukere. Den deterministiske
  banen er add_feature_request, requester upvote, players upvote, players downvote,
  update_status(Accepted), update_status(Completed). Scenarioet bruker den returnerte
  requesten; show_requests eller ekstra getters er ikke scenariokall.
- review: ekstern game_page kaller Game.add_review(user, title, message), som
  konstruerer Review og returnerer det nye objektet. Ingen ekstra scenariokall.

Dette er dokumenterte konkretiseringer og avvik fra ufullstendige tegninger,
ikke en påstand om verifisert samsvar med uendrede inputdiagrammer.

## Leveringsstatus

2026-10-13: alle seks klasser, tre tilstandsmaskiner, demo og fire scenarioer er
implementert. `cmake -S project/impl -B project/build` og
`cmake --build project/build` fullførte med exitkode 0 uten advarsler.
Planen ligger i `project/process/plans/completed/plan.md`.

UUID-aliasen innebærer ingen global UUID-generator: User får ID fra oppsettet,
og Game/Repository/Comment får deterministiske, typeprefiksede ID-er ved opprettelse.
Ingen programkjøring, tester eller UML-verifikasjon er utført. Avvikene dokumentert
under klarsignalet må tas med ved brukerens uavhengige vurdering av designet.
