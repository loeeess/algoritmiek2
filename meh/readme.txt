
Kopieer je eigen programma naar deze directory. Overschrijf daarbij
schema.h/schema.cc en (indien nodig) standaard.h/cc en constantes.h.

make

./TestSchema

In het keuzemenu kun je vervolgens met je eigen programma schema's
construeren. Als er een compleet schema wordt gevonden, wordt met
een aparte klasse Schema2 (met headers in schema2.h en constantes in
constantes2.h) gecontroleerd of het een geldig schema is.

Het bestand schema2.cc ontbreekt, maar is al wel gecompileerd, op
de huisuil01. Als je daar op inlogt (zie de website voor instructies),
zou het resultaat, schema2.o, te gebruiken moeten zijn. Wellicht ook op
andere Linux computers.

De memberfunctie controleerSchema van Schema2 controleert alle eisen
van een geldig schema. Als er schendingen van eisen worden gevonden,
worden daar ook passende foutmeldingen bij gegeven. Per soort eis wordt
er echter maar 1 foutmelding gegeven. Als er bijvoorbeeld tweetallen
spelers zijn die vaker dan twee keer tegen elkaar spelen in het schema,
dan wordt daar maar 1 foutmelding voor gegeven, ook als er meerdere
tweetallen spelers zijn waarvoor dat het geval is.

Uiteindelijk berekent controleerSchema ook een (straf)score. Als die
score 0 is, betekent het dat je een geldig schema had.
Tevens wordt aan het eind de waarde van het schema berekend: de waarde
die voor bepaalMinSchema geminimaliseerd moest worden, zoals beschreven
in de opdracht.

