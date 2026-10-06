# ADR 0001: Automatisk gjennomføring med uendrede inputmodeller

Dato: 6. oktober 2026. Status: Valgt implementasjonstolkning etter brukerens
instruksjon om å fortsette uten spørsmål. Ikke en endring av UML-fasiten.

## Beslutning

Gjennomfør planen uten godkjenningsstopp. Dokumenter minimale antakelser
og rapporter forskjeller mot input. Ikke endre diagrammer, filnavn eller verifier.

| Punkt | Valgt tolkning og konsekvens |
|---|---|
| A1 | Behold originalnavn. Standardverifikasjon vil fortsatt avvise to filer. Kjør i tillegg verifieren på byte-identiske kopier med støttede navn i en midlertidig mappe, med kopi av implementasjonen. Rapporter dette som separat kontroll. |
| A2 | `Date` er en typealias til C++20 `std::chrono::sys_days`, deklarert sammen med User. Ingen ekstra Date-klasse. Faste datoer brukes i demonstrasjonen. |
| A3 | Repository.download er public og returnerer en kopi av metadatamodellen. Ingen nettverksoverføring. |
| A4 | Behold readerens relasjonsretning og antall. Bruk rollenavn reviews, comments, comment, developers, repository og game. Disse navnene er antakelser fordi originalen ikke angir roller. Vektorer av weak_ptr representerer ikke-eiende samlinger; enkeltrelasjoner bruker råpekere. |
| A5 | Tilstandsmaskinen krever private state/suspension_active og public handle samt private no_active_suspension. Dette gir ekstra klasseelementer mot klassediagrammet og rapporteres. |
| A6 | Den løse teksten tolkes som ReopenAccount [no_active_suspension]. Sperreflagget bevares ved lukking. After30Days er en eksplisitt hendelse fra Closed til den antatte terminale tilstanden Deleted. Ingen faktisk klokke. Ny state/event/overgang er avvik fra readerens modell. |
| A7 | `Game_Review` tolkes som Review, uten å lage en ekstra anmeldelsesklasse. `game_page` blir en liten C++-controller med send_inn_review som kaller Game.add_review. Klassen/metoden er nødvendige antakelser fra sekvensen, men ekstra mot klassediagrammet. |
| A8 | Opprettelsesmetoder returnerer verdier som tegnet. De registrerer ikke pekere til midlertidige returverdier. Relasjonssamlinger kan settes via konstruktører og refererer til eksternt eide demoobjekter. create_repository, join_repository og remove_comment er plassholdere der livsløp/identitet ikke er spesifisert. Ingen skjult global lagring. |

## Omfang

edit_content endrer tekst; add_review og add_comment lager returverdier med
gitt tekst. download returnerer metadata. Ingen nye score-, request-,
autorisasjons-, database- eller grafikkfunksjoner innføres.

## Konsekvens

En byggbar og kjørbar mock-up kan leveres. 100 % alignment kan ikke loves
for motstridende eller uleselig input. Manglende alignment skal dokumenteres
som faktisk resultat, og planen flyttes ikke til completed så lenge dette gjenstår.

## Oppfølging av sekvensmelding

Metoden heter nå `send_inn_review`, som etter verifierens navnenormalisering
matcher «Send inn review». Game_Review er fortsatt en constructor-only
lifeline som ikke kan registreres av gjeldende runtime-verifikasjon.
