#!/bin/bash

# Déplacement dans le dossier build
cd ./build

# Compilation du projet
make

# Nécessaire pour la SFML
export LD_LIBRARY_PATH=../inc/SFML_2.4.2/lib

# Clear le terminal
clear

# Affichage de tous les fichiers textes disponibles
echo Fichiers textes disponibles :
ls *.txt

# Récupération du fichier texte voulu
echo Indiquer le nom du fichier texte à utiliser SVP :
read fichier

# Lancement de l'exécutable
./Hashi $fichier
