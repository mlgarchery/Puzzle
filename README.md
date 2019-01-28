# Installation

Installer SDL2, la librairie graphique 2D.

```bash
sudo apt install libsdl2-dev
```
puis

```bash
make
```


UTILE  
Echap:	quitter le puzzle  
clic gauche:	selectionner/déposer une pièce.  
clic droit:	appliquer une rotation de 90° à la pièce.  

JEU  
L'image est découpée en 16 pièces que l'on a mélangées.
Essayez de trouver sa disposition initiale.

PROGRAMMATION

-Dans notre main, nous avons : -La partie initialisation : création de la fenêtre, création du sprite, puis de la texture (contenant toute l'image) qu'on manipulera dans le jeu pour la création des pièces.
On définit une permutation des entiers de 1 à 16 aléatoirement en utilisant la fonction rand() et srand(time()) permettant le mélange des pièces à chaque exécution du programme.

Les pièces du puzzle ont:
- un rectangle, donnant sa position initiale sur l'image
- un rectangle de position actuelle sur l'ecran
- un rectangle definissant sa position si elle est en mouvement.
- un entierAngle caractérisant la rotation de la pièce (entierAngle*90°)
- un entier indiquant si la pièce est sélectionnée.

Différentes fonctions permettant de manipuler les pièces, ainsi que leurs affichage sont regroupées dans 
fonctionsPuzzle.h .

Ensuite, nous entrons dans la boucle principale (gestion des évenement) : 

-On teste les états des périphériques avec SDL_PollEvent

	cliquer sur la croix rouge ou appuyer sur échap pour quitter

	les évenements souris : On récupère la position de la souris
							Si une pièce est sélectionnée, alors, sa position suit celle de la souris.

							Le clic droit : on tourne la pièce de 90°

							Le clic gauche : Sélectionne la pièce par rapport aux coordonnées 			de la souris. Si une pièce est déja sélectionnée, elle est déposée dans la grille (ajuste les coordonnées de dépose pour coincider avec la grille)
							si une pièce est présente au moment de la dépose, on échange la place sur l'écran de celle-ci avec la dernière case sur laquelle se trouvait la pièce sélectionnée.


Enfin, nous effectuons un clear la fenêtre et nous affichons le puzzle mis à jour.

Dans l'affichage du puzzle mis à jour, nous nous assurons de render en dernier la pièce selectionnée afin qu'elle apparaisse au premier plan lors du déplacement.

On teste enfin, si le puzzle a été résolu. Si oui, on affiche une image de victoire et on quitte après un certain delay.
