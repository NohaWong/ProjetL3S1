# ProjetL3S1

Le programme <i>affichage_header</i> permet l'affichage de l'en-tête d'un fichier au format ELF 32 bits.

Valeurs de retour du programme :
  - 0 : Tout s'est bien passé
  - 1 : Les nombres magiques sont erronés
  - 2 : Le nombre d'arguments envoyés au programme est inccorect
  - 3 : Le fichier ELF n'est pas au format 32 bits
  - 4 : L'endianness du fichier n'est pas défini
  - 5 : La version du fichier ELF n'est pas valide
  - Autre : une erreur inconnue s'est produite