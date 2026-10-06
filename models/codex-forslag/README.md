# Forslag til felles modell — Open Game Library

**Status: AI-laget diskusjonsutkast, 6. oktober 2026. Ikke godkjent input/fasit.**

Dette er et forslag til å samle ideene i `models/hoglar/` og `models/Ole/`.
Originalene er ikke endret. Gruppen må ta stilling til valgene nedenfor før
dere lager den endelige modellen i `project/diagrams/input/`.
Dette er modellarbeid; ingen C++-implementasjon er laget.

## Start her

| Fil | Hva dere kan vurdere |
|---|---|
| [sammenligning.md](sammenligning.md) | Forskjeller, konkrete problemer og begrunnelsene for sammenslåingen |
| [beslutningsforslag.md](beslutningsforslag.md) | Foreslåtte ADR-beslutninger og spørsmål gruppen bør avklare |
| [class.drawio](class.drawio) | Felles klassemodell med åtte typer |
| [state-user.drawio](state-user.drawio) | Kontolivsløp fra e-postbekreftelse til logisk sletting |
| [state-request.drawio](state-request.drawio) | Behandling og ny innsending av funksjonsforslag |
| [sequence-write_review.drawio](sequence-write_review.drawio) | Gyldig anmeldelse fra en aktiv bruker |
| [sequence-feature_request_accepted.drawio](sequence-feature_request_accepted.drawio) | Nytt forslag, stemme, godkjenning og fullføring |
| [sequence-feature_request_resubmitted.drawio](sequence-feature_request_resubmitted.drawio) | Avvisning, redigering og ny innsending |
| [use-case.drawio](use-case.drawio) | Foreslått omfang for mock-upen |
| [activity-write-review.drawio](activity-write-review.drawio) | Anmeldelsesflyt med validering, avvisning og ny redigering |

Åpne `.drawio`-filene i VS Code. Klassediagrammet er stort; zoom inn på én
del av gangen. Blåkopien er en bevisst avgrensning, ikke en automatisk union
av alt dere har tegnet.

## Hvordan delene henger sammen

`User` representerer kontoen. Utvikler er foreslått som en rolle: en bruker
kan være skaper av et `Game` og bidragsyter i et `Repository`.
`Game` eier ett `Repository` og sine `Review`-objekter. Et repository eier
sine `Request`- og `RepoComment`-objekter. En anmeldelse eier sine
`ReviewComment`-objekter. Forfattere, skapere og bidragsytere refereres til
med assosiasjoner; disse objektene eier ikke brukerkontoene.

Request-delen er hovedsakelig hentet fra Ole, og anmeldelseskommentarene
og kontolivsløpet fra Hoglar. `Category` og score beholdes fra Ole.
Versjon, utgivelsesdato, e-post og profilinformasjon beholdes fra Hoglar.

Begge tilstandsmaskinene bruker `state: State` og `handle(event: Event)` i
klassediagrammet. `State`, `Event` og overgangstabellen skal senere ligge
inne i den aktuelle C++-klassen, slik malens verifikator forventer.

## Kontrakter som følger forslaget

- ID-er er `string`, datoer er ISO-datotekst (`YYYY-MM-DD`), og URL-er er
  `string`. Dette er foreslåtte mock-up-typer, ikke en ferdig UUID-/datoløsning.
- Klasseparametere som `User author` og `Game game` skal mappes til
  referanser i C++; objektene skal ikke kopieres for å lage relasjoner.
- Komposisjoner mappes til `unique_ptr` eller verdier. `*` blir en vektor.
  Assosiasjoner mappes til ikke-eiende pekere/referanser. Ingen `shared_ptr`
  er foreslått, siden ingen relasjon har hul diamant.
- `Game.add_review` kontrollerer `author.is_active()`, ikke-blank tittel og
  tekst og score 1–5. Ved feil returneres `nullptr`, og ingenting opprettes.
  Ved suksess opprettes `Review` med forfatter og fast demonstrasjonsdato,
  `Review.update` kalles, objektet lagres under spillet og en ikke-eiende
  peker returneres. Ingen virkelig klokke eller lagring er nødvendig.
- `Repository.add_feature_request` kontrollerer `author.is_active()` og
  ikke-blank tittel/tekst. Feil gir `nullptr`. Ved suksess opprettes en
  `Request` med forfatter, deterministisk ID, 0 stemmer og starttilstand
  `Pending`; `update` kalles, objektet eies av repositoryet og en peker returneres.
- `Review.update` setter tittel, tekst og score. `Request.update` setter
  tittel og tekst; når tilstanden er `Rejected`, kaller den
  `handle(Resubmit)`. Oppdatering i `Pending` endrer bare teksten.
  Endring av `Accepted`/`Completed` er foreslått ignorert i mock-upen.
- `upvote()` øker telleren og `downvote()` reduserer den; negative nettostemmer
  er tillatt. Ingen beskyttelse mot gjentatte stemmer er modellert.
- `add_comment` avviser inaktiv forfatter og blank tekst med `nullptr`.
  Ellers opprettes kommentaren med forfatter og tekst og eies av mottakeren.
  `remove_comment` fjerner bare et objekt i mottakerens egen samling;
  `edit_content` beholder gammel tekst ved blank input.
- `add_game_to_library`, `add_category` og `join` legger til bare dersom
  elementet ikke allerede er i samlingen. `join` gjelder bare aktive brukere.
  `show_requests` returnerer ikke-eiende pekere i innsettingsrekkefølge.
- `Game.download()` returnerer en fast mock-lenke for spillet.
  `Repository.download()` returnerer repositoryets URL. Ingen fil eller
  nettverksoverføring skjer. Dette er en forenkling av originalenes returtyper.
- Relasjonspekere lever så lenge eierobjektene lever. Brukerne opprettes før
  spillene og lever lengst i demonstrasjonen. `Deleted` er logisk sletting av
  kontoen i modellen, ikke destruksjon av et C++-objekt som kommentarer peker på.

Dette er **nye forslag til kontrakter** der originalene ikke var presise nok.
Godkjenn eller endre dem før spesifikasjon og implementasjon.

## Tilstander og scenarier

`User` starter i `AwaitingEmailVerification`, med `suspension_active = false`.
`ConfirmEmail` aktiverer kontoen. `Suspend` setter sperreflagget, og
`Reactivate` fjerner det. Lukking bevarer flagget, slik at en suspendert
konto ikke kan bli aktiv ved å lukke og gjenåpne. `Reopen` er bare tillatt
når `has_no_active_suspension()` er sann. `RetentionExpired` simulerer at
30 dager har gått og leder til `Deleted`; ingen klokke brukes. Hendelser
uten en passende overgang ignoreres. `is_active()` er en ren tilstandssjekk.
Forslaget dekker ikke fjerning av sperre mens kontoen er lukket.

`Request` starter i `Pending`. `Accept` leder til `Accepted`, `Reject` til
`Rejected`, `Resubmit` fra `Rejected` til `Pending`, og `Complete` fra
`Accepted` til `Completed`. Status kan ikke settes fritt med `update_status`.

Sekvensene viser konkrete forløp, med én lifeline per klasse:

1. **Anmeldelse:** aktiv forfatter, gyldig tekst/score og eksisterende spill.
   Aktør → Game.add_review → User.is_active → Review.update.
2. **Godkjent request:** aktiv forfatter og eksisterende repository.
   Aktør → Repository.add_feature_request → User.is_active → Request.update;
   deretter aktør → Request.upvote → Request.handle(Accept) → Request.handle(Complete).
3. **Ny innsending:** eksisterende `Pending`-request opprettes i scenarioets
   oppsett. Aktør → Request.handle(Reject) → Request.update;
   deretter selvkall Request → Request.handle(Resubmit).

Aktøren `Scenario` er en ekstern scenariodriver, ikke en C++-klasse. I
request-scenarioene representerer den handlinger fra spiller og utvikler
til ulike tider. Dette er en tilpasning til verifikatorens én-instans-per-klasse-
begrensning, ikke en påstand om at rollene har samme rettigheter.
Opprettelse skjer i konstruktører og er utelatt fra de sammenlignede meldingene.
Returpilene er dokumentasjon. Se beslutningsforslagene om autorisasjon.

## Avgrensning og kontroll

Søk, registreringsskjerm, publisering, faktisk Git-integrasjon, forks,
versjonshistorikk og donasjon er foreslått utsatt. Kontotilstandene er med,
men ikke en komplett innloggings-/registreringsløsning. Eksempelobjektene
kan opprettes direkte ved oppstart av mock-upen.

Klasse-, tilstands- og sekvensfilene er lest tilbake med repoets eksisterende
diagramlesere uten advarsler. Use case og aktivitet er supplerende dokumentasjon;
verifikatoren sammenligner ikke disse. Resultater fra visuell kontroll står i
[kontroll.md](kontroll.md).

**Ingen modell–kode-alignment er målt:** implementasjonen finnes ikke ennå.
At en diagramfil lar seg lese, viser ikke at domenebeslutningene er riktige,
at rettigheter er kontrollert, eller at en framtidig implementasjon vil gi 100 %.
