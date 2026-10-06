# Beslutninger gruppen bør ta

Alle punktene har status **foreslått**, ikke vedtatt ADR. Diagrammene i
denne mappen viser én konkret kombinasjon av valg som dere kan vurdere.

## ADR-utkast 1: Omfang for første mock-up

**Kontekst:** Konseptet omfatter både spillbibliotek, anmeldelser, bidrag,
Git, forks, versjoner og donasjon. De mest utviklede scenarioene handler
om anmeldelser og requests.

**Forslag:** Demonstrer anmeldelser/kommentarer, feature requests og
kontolivsløp med forhåndsopprettede lokale objekter. Nedlasting er en
lenkeplassholder. Ingen database, e-postutsending, betaling eller Git-klient.

**Konsekvens:** Et håndterbart omfang med konkrete sekvenser og to
tilstandsmaskiner. Forks og tilgjengelighet av alle versjoner i konseptteksten
er ikke oppfylt. Hvis disse er kjernefunksjonene, bør dere velge et annet
MVP-utsnitt før dere vedtar dette.

**Gruppens spørsmål:** Skal hovedhistorien være «spillere gir tilbakemelding»,
eller «spillere videreutvikler/forker spill»?

## ADR-utkast 2: Utvikler er en rolle

**Kontekst:** Hoglar bruker arv `Developer : User`, mens Ole bruker User
for alle og skiller roller i brukstilfeller.

**Forslag:** Én User-type. Game.creator og Repository.contributors viser
brukerens tilknytning. Profilfeltene bio/webpage_url er valgfrie tekstverdier
for alle kontoer. Utvikleraktøren i use case-diagrammet kan også gjøre
spillerhandlingene.

**Konsekvens:** En konto kan spille og bidra uten konvertering mellom
objekttyper. Dere har ikke modellert et globalt tilgangssystem eller
en godkjenningsprosess for å bli utvikler.

**Gruppens spørsmål:** Kan alle bidra, eller krever dette godkjenning?
Er det bare spillets skaper eller alle bidragsytere som kan håndtere requests?

## ADR-utkast 3: Eierskap og levetid

**Kontekst:** Modellene blander assosiasjon, aggregasjon og komposisjon,
og har ikke konsekvente rollenavn.

**Forslag:** Game eier Repository og Review. Repository eier Request og
RepoComment. Review eier ReviewComment. User-referanser og spillbiblioteket
er ikke-eiende. Brukere lever lengst i demonstrasjonen. Kontosletting
markeres som Deleted, uten å destruere objektet under eksisterende innhold.

**Konsekvens:** Tydelig C++-mapping uten delt eierskap. Sletting av et spill
vil destruere underinnholdet, så dere må velge om dette er ønsket.
En lokal Repository-modell er forskjellig fra det eksterne Git-repoet;
sletting i mock-upen skal ikke slette noe på GitHub.

**Gruppens spørsmål:** Skal anmeldelser og requests overleve at et spill
fjernes? Hvordan skal innhold fra lukkede/slettede kontoer vises?

## ADR-utkast 4: Hendelsesstyrte tilstander og deterministiske scenarioer

**Kontekst:** Fri `update_status` kan hoppe over lovlige overganger.
Tidshendelser og alternative sekvenser må tilpasses verifikasjonen.

**Forslag:** Bruk nested State/Event og `handle` med overgangstabell.
Skill godkjenning og avvisning i separate scenarioer. Tiden simuleres
med RetentionExpired. Ignorer hendelser uten en gyldig overgang.

**Konsekvens:** Enkelt å kontrollere mot tegningene, men ikke ekte tidsstyring.
Verifikatoren kontrollerer ikke alt: blant annet er argumentverdier og
autorisasjon ikke bevist av en riktig meldingsrekkefølge.

**Gruppens spørsmål:** Skal endring av en avvist request automatisk sende
den inn igjen, slik Oles sekvens antyder, eller må brukeren trykke «send på nytt»?
Skal Pending kunne trekkes tilbake? Kan Completed åpnes igjen?

## Avklar før implementasjon

1. **Autorisasjon:** Forslaget sjekker aktiv konto ved opprettelse, men
   har ingen håndheving av hvem som kaller Request.handle, update eller
   remove_comment. Scenarioene forutsetter autoriserte aktører. Enten vedta
   dette som en uttrykkelig mock-up-avgrensning, eller tegn hvem som sjekker
   rettigheter og hvilke brukere som sendes med. Ikke hevde at kontostatus
   alene beskytter systemet.
2. **Stemmer:** `votes: int` og upvote/downvote kan ikke sikre én stemme per
   bruker. Vedta en ren teller, eller modeller stemmeidentitet og nødvendige
   kall. Bestem også om stemmer kan endres og når avstemningen stenger.
3. **Anmeldelser:** Forslaget bruker score 1–5 og ikke-blank tekst. Er dette
   riktig? Én anmeldelse per bruker/spill er ikke håndhevet. Må brukeren
   ha spillet i biblioteket først, og kan andre redigere/slette?
4. **Konto:** Lukkede, tidligere suspenderte kontoer kan ikke gjenåpnes i
   dette utkastet. Hvem kan oppheve sperren? Skal ubekreftet konto kunne lukkes?
   «Deleted» betyr logisk sletting her; ingen anonymisering er modellert.
5. **Typer og avgrensning:** Godta tekst-ID/dato, faste demodata og manglende
   versjonshistorikk, eller spesifiser sterkere typer og et annet MVP-utsnitt.
6. **Rekkefølge:** Vedta modellene og ADR-ene først. Deretter skrives endelig
   spesifikasjon og implementasjonsplan med kontrollpunkter. Disse filene
   er ikke en automatisk godkjenning til å starte kodegenerering.
