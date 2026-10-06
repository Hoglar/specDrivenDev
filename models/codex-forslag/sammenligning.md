# Sammenligning av gruppens modeller

Gjennomgått 6. oktober 2026. Kildene er alle de ni `.drawio`-filene i
`models/hoglar/` og `models/Ole/`, samt de to tekstfilene der.
Dette dokumentet skiller observasjoner fra foreslåtte endringer.

## Kildeoversikt

| Kilde | Innhold og bidrag |
|---|---|
| `hoglar/firstClass.drawio` | User, Developer, Game, Review, Repository, Review_Comment og Repo_Comment; utviklerarv, profiler, versjon/dato og kommentarer |
| `hoglar/state-user.drawio` | E-postbekreftelse, aktiv, suspendert, lukket, gjenåpning og avslutning etter 30 dager |
| `hoglar/gameReviewSequence.drawio` | Spiller via game_page til Game og opprettelse av Game_Review |
| `hoglar/writeReviewActivity.drawio` | Skrive/sende anmeldelse og kontrollere teksten; begge beslutningsgrener mangler mål |
| `hoglar/useCaseFirst.drawio` | Bruker/utvikler, registrering, søk, nedlasting, anmeldelser, publisering/republisering og repo-kommentarer |
| `hoglar/first.md` | Inneholder bare «Hei»; ingen beslutninger å bygge videre på |
| `Ole/class.drawio` | User, Game, Review, abstrakt Comment, Repository, Request og enumene Category/Request_Status |
| `Ole/request-state.drawio` | Pending, Accepted, Rejected og Completed med mulighet for oppdatering etter avvisning |
| `Ole/feature-request-sequence.drawio` | Opprette, stemme, godkjenne/fullføre eller avvise og sende inn request på nytt |
| `Ole/use-case.drawio` | Bibliotek, spillnedlasting, anmeldelser, donasjon, browsing, bidrag og requests |
| `Ole/adr.md` | Konsept om åpen spillutvikling med Git, tilgjengelig kildekode, versjoner og forks; foreløpig konseptbeskrivelse, ikke konkrete beslutningsposter |

## Samordning

| Tema | Hoglar | Ole | Forslaget |
|---|---|---|---|
| Identitet | `int`, `user_id`, `game_id` | `uuid`, `id` | `string id` i mock-up; UUID er ikke en standard C++-type |
| Navn | `user_name`, `title`, `content`, `String` | `username`, `name`, `description`, `string` | `username`, `title`, `description` for review/request; `content` for kommentarer; `string` |
| Utvikler | `Developer` arver `User` | Alle er `User`; utvikler er aktør | Én `User`, roller via Game.creator og Repository.contributors |
| Profil | E-post, bio og nettside | Brukernavn | Feltene beholdes samlet på User |
| Anmeldelse | Innhold og dato, med kommentarer | Score og fellestrekk fra Comment | Selvstendig Review med tittel, tekst, score, dato, forfatter og kommentarer |
| Kommentar | Review_Comment og Repo_Comment er svar på innhold | Comment er en abstrakt base for review/request | Behold to konkrete kommentartyper; utsett felles baseklasse |
| Feature request | Ikke med i klassediagrammet | Request med stemmer og status | Behold Request og livsløpet |
| Spill/repo | Repository koblet til Game | Game komponerer Repository | Behold Game som eier av ett lokalt repository-objekt |
| Eierskap til innhold | For det meste assosiasjoner | Aggregasjon til Review og Request | Foreslå komposisjon fra beholder til innhold; krever gruppens godkjenning |
| Bibliotek | Ikke eksplisitt klassemedlem | User.game_library | Ikke-eiende samling av Game-referanser |
| Kategorier | Ikke med | Category med fem navn og `...` | Behold de fem konkrete verdiene; fjern plassholderen |
| Konto | Egen tilstandsmaskin, ikke uttrykt i User-klassen | Ingen maskin | Behold og knytt maskinen til User.state/handle |
| Requests | Ingen maskin | Request_Status og update_status | Nested State/Event og hendelsesstyrt handle; ingen duplisert status-enum |
| Nedlasting | Returnerer Game/Repository | Use cases | Returnerer mock-lenker; ingen reell nedlasting |

## Konkrete ting å rette i originalenes videre utvikling

### På tvers av modellene

- **Review og Game_Review er forskjellige navn.** Hoglars klasse- og
  sekvensdiagram bør bruke samme navn. Forslaget bruker `Review`.
- **game_page mangler i klassediagrammet.** Enten må dere tegne en klasse
  med en definert metode, eller la sekvensens eksterne aktør kalle `Game`
  direkte. Forslaget velger direkte kall for å begrense mock-upen.
- **Score kan ikke settes tydelig i Oles API.** `add_review` har ingen
  score-parameter, og `update_score()` har heller ingen verdi inn.
  Forslaget fører score eksplisitt gjennom `add_review` og `Review.update`.
- **Kontolivsløpet må synes i User-klassen.** Tilstandsattributt, handle,
  guard og nødvendige actions er lagt til i forslaget.
- **Review/Request-relasjoner bør ikke dobles som attributter.** Oles
  `reviews`, `feature_requests` og `game_repository` er både skrevet som
  klassefelter og tegnet som relasjoner. I malens mapping representerer
  relasjonen allerede feltet. Forslaget tegner hvert felt én gang.

### UML-notasjon og eierskap

- Oles `Comment {abstract}` leses ikke som et gyldig klassenavn av
  verifikatoren. De to pilene fra Comment har heller ikke den hule
  trekanten som uttrykker arv. Dersom dere beholder basen, bruk støttet
  abstrakt-notasjon, pil mot basen og en meningsfull ren virtuell metode
  etter repoets mapping. Ikke innfør en kunstig metode bare for rapportens skyld.
- Noen av diamantene i Oles modell plasserer eierskap hos innholdet i
  forhold til User. En forfatter er normalt referert til fra Review/Request;
  vedkommende er ikke en del som eies av anmeldelsen eller spillet.
- Hoglars forbindelser mangler i stor grad rollenavn og navigeringspiler,
  og noen har flere motstridende etiketter. Readeren velger da en retning
  fra XML-ens source/target, selv om den visuelle forbindelsen er urettet.
  Tegn bevisst retning, rolle og én relevant multiplisitet per ende.
- Verifikatoren leser `Repository.download(): Repository` i Hoglars modell
  som uleselig fordi synlighetsmarkør mangler. Bruk eksplisitt `+`/`-`/`#`.
- Avklar om et Git-repo kan leve uavhengig av spillet. Forslaget lar Game
  eie **den lokale metadatamodellen**, ikke det eksterne GitHub-repoet.

### Tilstandsdiagrammer

- Oles piler Pending → Accepted, Pending → Rejected og Accepted → Completed
  mangler hendelser som diagramleseren finner på de respektive pilene.
  Flere `update_status()`-etiketter ligger på en annen forbindelse.
  Forslaget bruker de entydige hendelsene `Accept`, `Reject`, `Complete`
  og `Resubmit` og fester hver etikett til riktig pil.
- Hoglars «Reopen account [no active suspension]» er en løs tekst, ikke
  en etikett på overgangen. Leseren finner derfor en overgang uten event.
  Forslaget gjør dette til `Reopen [has_no_active_suspension]` på pilen.
- `After(30 days)` til sluttsymbolet kan ikke mappes direkte av verifikatoren.
  Forslaget bruker `RetentionExpired` til en eksplisitt `Deleted`-tilstand.
  En deterministisk hendelse representerer tidsforløpet i mock-upen.
- En sperret konto må ikke kunne omgå sperren ved Close → Reopen.
  Forslaget beholder et sperreflagg. Hvem som kan oppheve en sperre og
  hva som skjer ved lukking før e-postbekreftelse, er fortsatt designvalg.

### Sekvenser og aktivitet

- Oles request-sekvens har tre `User`-lifelines. Verifikatoren skiller
  klasser, ikke ulike objekter av samme klasse. Den varsler og hopper over
  flere meldinger. Dette er en verktøybegrensning; flere User-objekter er
  ikke i seg selv feil UML.
- `alt` er gyldig UML, men støttes ikke som alternative kjøringer her.
  Forslaget deler forløpene i to filer: godkjent/fullført og avvist/innsendt på nytt.
- `add_feature_request(title, description)` i originalsekvensen mangler
  User-argumentet fra klassesignaturen. Forslaget viser `author` eksplisitt.
- Konstruktorer og create-meldinger sammenlignes ikke med runtime-tracen.
  Forslaget beskriver opprettelse i kontraktene og tegner bare vanlige
  metodekall. Ingen lifeline blir stående kun for en konstruktor.
- Aktivitetsdiagrammets ja/nei-grener stopper i løse piler. Forslaget
  fullfører begge: lagre/bekrefte, eller vise feil og redigere på nytt.

## Ting som er foreslått utsatt, ikke glemt

| Originalidé | Hvorfor utsatt / hva som trengs om dere beholder den |
|---|---|
| `Developer.create_repository`, `User.create_new_game`, publisere spill | Krever felles beslutning om opprettelse, eier og returverdi. Mock-upen starter med ferdig opprettede spill/repoer. |
| Registrer som utvikler | Passer ikke direkte med rolleforslaget; avklar om alle kan bidra eller må godkjennes. |
| Brukerregistrering | Kontotilstandene beholdes, men registrerings-/autentiseringsflyt er ikke modellert. |
| Søk/browse | Trenger katalogansvar, søkeparametere og resultattyper. |
| Donasjon | Trenger mottaker, beløp og tydelig betalingsmock; kan droppes fra første MVP. |
| Fork/republisering og alle versjoner | Trenger modell for versjon og opprinnelse. Ett `version`-felt oppfyller ikke hele konseptet i Oles tekst. |
| Faktisk nedlasting og Git-integrasjon | Ligger utenfor en mock-up med lenker og lokale objekter. |
| Generisk abstrakt Comment-base | Originalene bruker «Comment» om ulike ting. Diskuter domenesemantikken før arv. |
| `update_score` / `update_status` som separate operasjoner | Score inngår i Review.update; fri statussetting erstattes av kontrollerte hendelser. |

Hvis forks og versjoner er selve poenget dere ønsker å demonstrere,
bør dere prioritere det foran kommentarer og donasjon. Da må dette
utkastet endres: dagens forslag vektlegger anmeldelser og feature requests,
fordi det er der dere allerede har konkrete sekvenser og tilstandsmodeller.
