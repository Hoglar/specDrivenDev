# Spesifikasjon for AI-implementasjon av UML

## Oppgave

Implementer UML-designet i `project/diagrams/input/` som C++20-kode i
`project/impl/`. Implementasjonen skal samsvare med designet, uten å legge til
funksjonalitet, klasser, medlemmer eller relasjoner som ikke er tegnet, med
unntakene som er uttrykkelig tillatt i `AGENTS.md`.

Denne filen beskriver rammene for implementasjonsarbeidet. De konkrete klassene,
tilstandsmaskinene og scenarioene defineres av UML-diagrammene.

## Kilder og avgrensning

- Les `AGENTS.md` i repoets rot før implementasjonsarbeidet begynner. Følg reglene
  der for filstruktur, UML-til-C++-mapping og bygging.
- Bruk `project/diagrams/input/class.drawio` som kilde for klassedesignet.
- Implementer også tilstandsmaskiner og scenarioer dersom det finnes filer med
  navnene `state-<class>.drawio` og `sequence-<scenario>.drawio` i samme mappe.
- Filene i `project/diagrams/input/` er skrivebeskyttet for AI-agenten. Ikke
  endre, slette eller erstatte dem.
- Les designet direkte fra inputdiagrammene. Ikke bruk genererte diagramutdata
  eller verifikasjonsrapporter som grunnlag for implementasjonen.
- `adr.md` i denne mappen gir bakgrunn for konseptet. Ikke utled ekstra
  implementasjonskrav fra konseptbeskrivelsen som ikke finnes i UML-designet.

Arbeid innenfor `project/`. Nødvendige unntak er å lese `AGENTS.md`,
mappingdokumentet `tools/umlverify/docs/UML-CPP-MAPPING.md` og eksemplene som
`AGENTS.md` viser til, samt å kjøre CMake og kompilatoren for vanlig bygging.
Ikke endre filer utenfor `project/`.

Skriv implementasjonen bare i `project/impl/`, og spesifikasjoner og arbeidsplaner
i `project/process/`. Byggfiler kan genereres ved vanlig kompilering.

## Verifisering er en separat fase

Verifisering utføres av oppdragsgiver etterpå, uavhengig av implementeringen.
AI-agenten skal ikke kjøre verifikatoren, verifikasjonsoppgaver i VS Code,
sporing eller andre sammenligninger mellom kode og UML. Dette gjelder også
kjøring av verifikatoren bare for å konvertere diagrammer.

Ikke generer, les eller bruk verifikasjonsrapporter og sammenligningsdiagrammer
som tilbakemelding under implementeringen. Ikke tilpass koden til tidligere
verifikasjonsresultater. Følg inputdiagrammene og mappingreglene direkte.
Vanlig kompilering for å avdekke byggefeil og kompilatoradvarsler er tillatt.

## Krav til implementasjonen

1. Behold designets navn, typer, synlighet, relasjoner og multiplisiteter. Bruk
   C++-mappingen i `AGENTS.md` og mappingdokumentet.
2. Følg den påkrevde mappestrukturen med headere i `include/`, implementasjon i
   `src/`, en `demo` og ett scenario-program per sekvensdiagram i `scenarios/`.
3. Implementer tilstandsmaskiner som overgangstabeller og scenarioer med nøyaktig
   de tegnede kallene i riktig rekkefølge, slik `AGENTS.md` beskriver.
4. Ikke legg til egne designforbedringer eller foreslå forbedringer av koden.
   Retting av avvik fra diagrammene og byggefeil er en del av oppgaven.
5. Hvis kildene er motstridende eller mangler informasjon som er nødvendig for
   implementasjonen, beskriv det konkrete problemet og be om avklaring. Ikke
   gjett nye krav eller endre diagrammene. Fortsett med uavhengige deler som er
   tilstrekkelig spesifisert.

## Arbeidsflyt

1. Les instruksjonene og alle relevante inputdiagrammer.
2. Før kode skrives, dokumenter designets klasser, medlemmer, relasjoner,
   tilstander, hendelser, overganger og scenariokall, med tilhørende C++-mapping,
   i `project/process/specs/`.
3. Lag en sjekkliste i `project/process/plans/YYYY-MM-DD-<tema>.md`.
4. Implementer designet i `project/impl/`.
5. Bygg implementasjonen med CMake og rett eventuelle byggefeil og advarsler.
6. Lever implementasjonen og dokumenter eventuelle uavklarte punkter.
   Verifisering inngår ikke i denne arbeidsflyten.

## Ferdigkriterier

- Implementasjonen bygger som C++20 uten advarsler.
- Alle tilgjengelige klasse-, tilstands- og sekvensdiagrammer er implementert.
- Inputdiagrammene er uendret.
- Fullførte planer er flyttet til `project/process/plans/completed/`.

Avslutt med en kort beskrivelse av hva som ble implementert, byggestatus og
eventuelle blokkeringer. Opplys at verifisering ikke er utført, og ikke påstå
verifisert samsvar. Implementasjonsarbeidet kan avsluttes før den uavhengige
verifiseringen gjennomføres.
