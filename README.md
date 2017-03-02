#Slardar

UTILE  
Echap:	quitter le puzzle  
clic gauche:	selectionner/déposer une pièce.  
clic droit:	appliquer une rotation de 90° à la pièce.  

JEU  
L'image est découpée en 16 pièces que l'on a mélangées.
Essayez de trouver sa disposition initiale.

PROGRAMMATION

-Dans notre main, nous avons : -La partie initialisation : création de la fenêtre, création du sprite, puis de la texture qu'on manipulera dans le jeu, découpe du puzzle et permutations.

Les pièces du puzzle ont un angle, un rectangle, donnant sa position initiale, et un rectangle de position actuelle; et un indicateur de sélection.

Ensuite, nous entrons dans la boucle principale : 

-On teste les états des périphériques avec SDL_PollEvent

	cliquer sur la croix rouge ou appuyer sur échap pour quitter

	les évenements souris : On récupère la position de la souris
							Si une pièce est sélectionnée, alors, sa position suit celle de la souris.

							Le clic droit : on séléctionne la pièce puis on la tourne de 90°

							Le clic gauche : Sélectionne la pièce par rapport aux coordonnées 				de la souris.	
											si une pièce est déja sélectionnée, la dépose dans la grille (ajuste les coordonnées de dépose pour coincider avec la grille)
											si une pièce est présente au moment de la dépose, on l'échange avec celle qui était sélectionnée.


Enfin, nous clearons la fenêtre et nous affichons le puzzle mis à jour.

La dernière boucle for permet d'afficher la pièce sélectionnée au premier plan.

On teste enfin, si le puzzle a été résolu, si oui, on affiche une image de victoire et on quitte.