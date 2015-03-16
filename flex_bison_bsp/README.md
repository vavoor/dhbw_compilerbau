Dieses Beispiel soll die Benutzung von flex und bison illustrieren.

Als Beispiel dient eine Sprache, die endliche Automaten z.B. für die Steuerung
von Anlagen definiert. Die Sprache sieht so aus:


# initial state is state unlocked
initial state unlocked:
  control light_off         # A state can control something
  control door_unlocked
  on enter goto locked      # On certain events, a state transition can happen


state locked:
  control light_on
  control door_locked
  on exit goto unlocked
  on emergency_button goto emergency

state emergency:
  control alarm_bell;
###############################################################


Schlüsselwörter sind: inital, state, control, on, goto.

Auf das Schlüsselwort state folgt der Name des Zustands.
Auf das Schlüsselwort on folgt der Name eines Ereignisses.
Auf das Schlüsselwort goto folgt der Name des Zustands, in den beim Eintritt
des Ereignisses gewechselt wird.

Zustandsnamen und Ereignisnamen bestehen aus Buchstaben, Zahlen und Underscores.
Schlüsselwörter dŸrfen nicht als Zustands- oder Ereignisnamen benutzt werden.