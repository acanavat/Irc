Irc

Un projet en C++ permettant de se connecter à un serveur IRC, d’envoyer et de recevoir des messages, et de gérer les canaux IRC.

📣 Introduction

Ce projet implémente les bases du protocole IRC (Internet Relay Chat) en C++, avec une structure simple et claire. Il peut servir de client IRC minimal ou de base pour construire un bot IRC, un outil réseau ou un client plus complet.

Le projet est constitué principalement de deux fichiers source (irc.hpp, irc.cpp) et d’un Makefile pour la compilation.

🚀 Fonctionnalités

Connexion à un serveur IRC

Envoi et réception de messages IRC

Support des commandes de base : JOIN, PRIVMSG, etc.

Architecture extensible

🛠 Installation
git clone https://github.com/acanavat/Irc.git
cd Irc
make

🎯 Exemple d’utilisation avec irssi

Tu peux utiliser irssi
 comme client IRC pour tester ton serveur (ou t'interfacer avec un serveur distant si ton programme agit comme client).

🔹 Étapes :
1. Lancer irssi
irssi

2. Se connecter à ton serveur local (ou distant)

Si ton programme lance un serveur IRC local écoutant sur le port 6667, tu peux te connecter ainsi :

 /connect 127.0.0.1 6667

3. Rejoindre un canal IRC
/join #test

4. Envoyer un message
/msg #test Hello IRC!


Cela te permettra de voir comment ton code interagit avec un client IRC réel comme irssi.

🧠 Structure du projet
├── irc.hpp       # Déclaration des classes et fonctions IRC
├── irc.cpp       # Implémentation de la logique IRC (connexion, communication, etc.)
├── Makefile      # Compilation simple
└── README.md     # Description du projet

🌏 Meta:
rbulanad : rbulanad@student.42nice.fr
acanavat : acanavat@student.42nice.fr
