
# Projet BDM

## Installation

Ce projet utilise [`OpenCV 3`](http://opencv.org/) et [`Qt5`](https://www.qt.io/). Il faut installer ces deux bibliothèques pour pouvoir compiler. Il faudra également `qmake` (inclus avec Qt) et un compilateur c++ supportant c++11.

D'abord, clonez le dépôt :

    git clone https://github.com/alaenix/BDM.git

Puis, pour compiler le projet :

    qmake
    make

Et pour le lancer :

    ./BDM

## Fonctionnement

Le programme detecte le texte de l'image avec Tesseract. Puis, il le traduit en interrogant le service de traduction de Microsoft. Enfin, le texte est remplacé par la traduction. Afin de respecter les couleurs de l'image, une binarisation d'Otsu est appliquée pour extraire la couleur du texte de la couleur du fond.

