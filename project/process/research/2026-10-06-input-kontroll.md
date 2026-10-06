# Fase 1: kontroll av input

Dato: 6. oktober 2026.
Status: Gjennomgang utført. Modellavklaringer og kontrollpunkt 1 gjenstår.

## Utført

- Lest gjeldende plan og spec samt alle tre input-diagrammene med repoets
  diagramlesere. Klassediagrammet inneholder sju lesbare klasser og ni relasjoner.
- Kjørt `.venv/bin/python tools/verify.py project`. Kommandoen avsluttet
  med exit-kode 1 fordi klasse- og sekvensfilenes navn ikke støttes.
- Lest `project/reports/state-user-report.md`. Ingen alignment er målt:
  rapporten oppgir at implementasjonen mangler.
- Input-filene skal beholde samme innhold gjennom AI-arbeidet.
  Ingen modellvalg eller endringer er gjennomført av AI-en.

## Avklaringsliste for brukeren

Dette konkretiserer A1–A8 i specen. Forslagene er ikke vedtatt.

| ID | Observasjon | Neste handling eller valg |
|---|---|---|
| A1 | Verifikatoren avviser `firstClass.drawio` og `gameReviewSequence.drawio`. | Brukeren gir filene navnene `class.drawio` og `sequence-write_review.drawio`. |
| A2 | `Date` brukes i Game og Review uten definert type. | Bestem en konkret C++-mapping i en ADR og samordne modellen ved behov. |
| A3 | `Repository.download(): Repository` mangler synlighetsmarkør og hoppes over av diagramleseren. | Brukeren angir ønsket synlighet på metoden. |
| A4 | Assosiasjonene mangler rollenavn/navigeringspiler; enkelte antallsmerker spriker. | Avklar antall, retning og navn på relasjonsmedlemmene. Ikke endre eierskap bare for å forenkle implementasjonen. |
| A5 | User-klassen mangler state-attributt, handle-metode og gjenåpningsguard. | Brukeren samordner User-klassen med den ønskede tilstandsmaskinen. |
| A6 | Closed → Active mangler event-etikett på selve pilen. Tidsmerket pil til sluttsymbol støttes ikke av mappingen. | Fest gjenåpningshendelse/guard til pilen og avklar representasjonen av 30-dagersovergangen og sperredata. |
| A7 | Sekvensen bruker Game_Review og game_page, mens klassediagrammet bare har Review. Første melding har ikke et navngitt API. | Velg felles anmeldelsesnavn. Avklar om game_page skal modelleres som klasse, eller om aktøren skal kalle Game direkte. |
| A8 | Metoder oppretter/returnerer objekter samtidig som relasjonene er ikke-eiende. | Avklar hvem som holder objektene i live og hvordan relasjonene etableres. |

Create-meldingen i sekvensen sammenlignes ikke, slik mappingen dokumenterer.
Dette er en verktøybegrensning; konstruktørens opprettelse må fortsatt ivaretas.

## Kontrollpunkt 1

Fase 1 er ikke ferdig bare fordi verifikatoren er kjørt. Når brukeren har
avklart punktene, leses modellene igjen, beslutninger dokumenteres og specen
oppdateres. Verifikatoren må da oppdage alle tre diagramtypene og lese de
nødvendige elementene før brukeren godkjenner overgangen til fase 2.

Ingen C++-kode er generert. Ingen planpunkter for modellavklaring eller
100 % alignment er markert som fullført.
