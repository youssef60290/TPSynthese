# Rapport de TP : Client TFTP

## **Introduction**
Dans ce TP, j’ai dû programmer un client TFTP (Trivial File Transfer Protocol) capable de télécharger des fichiers depuis un serveur en utilisant le protocole UDP. Ce TP m’a permis de combiner mes connaissances en programmation système et en réseaux, tout en approfondissant des concepts essentiels comme la gestion des sockets, la création de paquets et la communication avec un serveur distant.

---

## **Question 1 : Gestion des arguments**

### **Ce que j’ai fait**
J’ai d'abord appris à récupérer les arguments passés en ligne de commande pour configurer mon client. Ces arguments comprenaient l'adresse du serveur, le port et le nom du fichier à télécharger.

### **Ce que j’ai appris**
- L’importance de valider les arguments fournis pour éviter les erreurs d'exécution.
- Comment structurer un programme pour traiter dynamiquement des entrées utilisateur.

### **Ressenti**
Cette étape était simple mais essentielle pour poser les bases du projet. Cela m’a aidé à comprendre qu’un programme robuste commence par une bonne gestion des entrées utilisateur.

---

## **Question 2 : Résolution d’adresse (getaddrinfo)**

### **Ce que j’ai fait**
J’ai utilisé `getaddrinfo` pour résoudre l’adresse IP du serveur à partir de son nom de domaine ou de son adresse symbolique (comme `localhost`). Cela m’a permis d’obtenir une structure contenant les informations nécessaires pour ouvrir un socket.

### **Ce que j’ai appris**
- Le rôle de `getaddrinfo` dans la résolution d’adresses symboliques en IPv4 ou IPv6.
- Comment extraire et afficher les informations résolues (adresse IP et port) pour s'assurer que tout fonctionne correctement.

### **Ressenti**
Comprendre le fonctionnement de `getaddrinfo` m’a permis de mieux appréhender les interactions entre les noms de domaine et les adresses IP dans les applications réseau.

---

## **Question 3 : Création et gestion des sockets**

### **Ce que j’ai fait**
J’ai ouvert un socket en mode UDP pour établir la communication avec le serveur TFTP. J'ai également appris à gérer correctement les erreurs lors de la création du socket.

### **Ce que j’ai appris**
- Comment créer un socket avec `socket()` et configurer les options pour UDP.
- Les différences entre TCP et UDP en termes de fiabilité et de performance.

### **Ressenti**
La création d’un socket semblait simple au début, mais cela m’a permis de comprendre comment les protocoles réseau s’appuient sur ces mécanismes pour fonctionner.

---

## **Question 4a : Envoi de la requête RRQ**

### **Ce que j’ai fait**
J’ai construit une requête de lecture (RRQ) conforme à la RFC1350, contenant l’opcode, le nom du fichier et le mode de transfert ("octet"). Cette requête a été envoyée au serveur avec `sendto`.

### **Ce que j’ai appris**
- Comment construire un paquet en respectant un format spécifique.
- L’utilisation de `sendto` pour envoyer des données via un socket UDP.

### **Ressenti**
Cette étape m'a permis de mieux comprendre les bases des protocoles applicatifs comme TFTP. Avec l'aide de **M. Tauvel**, j'ai appris à utiliser Wireshark pour vérifier que la requête était correctement formatée.

---

## **Question 4b et 4c : Réception des paquets DAT et envoi des ACK**

### **Ce que j’ai fait**
J’ai reçu des paquets de données (DAT) du serveur avec `recvfrom`, tout en vérifiant l’opcode et le numéro de bloc. Chaque paquet reçu était confirmé par l’envoi d’un acquittement (ACK) via `sendto`. Les données ont été sauvegardées dans un fichier local.

### **Ce que j’ai appris**
- Comment analyser les paquets reçus (opcode et numéro de bloc).
- L’importance de répondre rapidement avec des ACK pour assurer le bon fonctionnement du protocole.
- Utiliser des boucles pour gérer plusieurs paquets et détecter la fin du transfert.

### **Ressenti**
Cette partie était complexe, car il fallait gérer plusieurs paquets de manière synchrone. **M. Tauvel** m’a énormément aidé à comprendre les messages ICMP et les erreurs de port non atteignable lorsque le serveur TFTP n’était pas correctement configuré.

---

## **Conclusion**

### **Résumé**
Ce TP m’a permis d’acquérir une meilleure compréhension de la programmation système et réseau. J’ai appris à :
- Manipuler les sockets pour envoyer et recevoir des paquets UDP.
- Implémenter un protocole applicatif simple (TFTP) en respectant ses spécifications.
- Utiliser des outils comme Wireshark pour analyser et déboguer les communications réseau.

### **Ressenti personnel**
Ce TP était enrichissant mais demandait beaucoup de rigueur. Il m’a appris que même les détails, comme un mauvais port ou une requête mal formatée, peuvent bloquer tout le fonctionnement d’un programme réseau. Avec l’aide de **M. Tauvel**, j’ai pu relever le défi et progresser. Merci encore pour son soutien et ses explications claires !

