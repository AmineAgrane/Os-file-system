/*!
 * \file head.h
 * \brief Header contenant les constantes ainsi que la structure utilisée dans notre programme
 * Contient aussi la signature de l'ensemble des fonctions utilisées
 * \author Agrane Amine 40% / Khelfane Lydia 30% / Metidji Fares 30%
 * \version 1
 * \date 28 mars 2019
 */

#ifndef HEADER
#define HEADER

#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
//#include <my_linux_types.h>

#define NOMBREBLOCS 300          // Nombre de blocs dans la partition
#define TAILLEBLOC 560   		 // Taille de chaque Bloc
#define MAXNOMFICHIER  20		 // Taille maximale pour le nom d'un Bloc
#define TAILLEREPERTOIR 25       // Nombre de documents (Fichier ou repertoir) qu'un repertoir peut contenir
const int tab_init[25] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

// Taille maximale de la donnée que le bloc peut contenir
#define MAXDONNEEBLOC (TAILLEBLOC - 5*sizeof(int) - sizeof(int[15])  - sizeof(char[MAXNOMFICHIER]))

/**
 * \struct Bloc
 * \brief Structure d'un bloc (La partition est une sucession de N blocs).
 * Contient les différents élément qui composent un bloc de la partition
 * Un bloc peut représenter un fichier ou un dossier. Si un fichier est trop gros pour
 * tenir sur un seul bloc, on effectue sa décomposition en N blocs nécessaire pour stocker son contenu
 */

typedef struct Bloc
{
  //  Indique si le bloc représente un fichier ou un dossier (1 => fichier 0 => dossier)
  int FICHIER_DOSSIER;
  //  Identifiant unique du bloc à l'intérieur de la partition
  int ID_BLOC;
  //  Indique si le bloc est occupé ou non (1 => Libre 0 => occupé)
  int LIBRE;
  //  Le nom du Bloc (Nom du fichier ou du dossier)
  char NOM_BLOC[MAXNOMFICHIER];
  //  L'identifiant unique du répertoir Parent d'un fichier ou d'un dossier
  int PERE;
  //  Les données stockées sur le bloc(Contenu d'un fichier)
  char DONNEES[MAXDONNEEBLOC];
  /*  Tableau d'identifiants    de bloc, les identifiant se trouvant dans ce bloc
  représentent les éléemnt contenu à l'intérieur du fichier représenté par ce bloc
  L'ensemble du tableau vaut -1 dans le cas d'un fichier */
  int LIENS[TAILLEREPERTOIR];
  /*  Contient l'identifiant du bloc qui contient la suite du fichier
  Prends -1 si le fichier est contenu dans un seul bloc ou si le bloc représente un dossier */
  int BLOC_SUIVANT;
}Bloc;

//Signatures fonctions et procédures
static void erreur(char *message);
void LSEEK(int partition, int NumeroBloc);
Bloc READ(int partition, int NumeroBloc);
void WRITE(int partition, int NumeroBloc, Bloc bloc);
int  PremierBlocVide(int partition);
void AffichageBloc(Bloc bloc);
Bloc InitBloc(int fd, int id, int libre, int suiv, char *nom, char *donnees, int pere);
void AffichageContenuDossier(int partition, int RepertoirCourant);
int  IdNomElement(int partition, int RepertoirCourant, char nom[MAXNOMFICHIER]);
int  AjouterLien(int partition, int RepertoirParent, int identifiant);
int  SupprimmerLien(int partition, int RepertoirParent, int lien);
void NomBloc(int partition, int NumeroBloc, char *nombloc);
void SupprimmerElementRepertoir(int partition, int RepertoirParent, char *nom);
void SupprimmerDossier(int partition,  int RepertoirParent, int NumeroBloc);
void SupprimmerFichier(int partition, int RepertoirParent, int NumeroBloc);
void AffichageContenuFichier(int partition, int RepertoirCourant, char *nom);
void AffichagePartition(int partition);
int  CreationDossier(int partition, int RepertoirParent, char *nom_dossier);
int  ElementExisteRepertoir(int partition, int RepertoirCourant, char nom[MAXNOMFICHIER]);
void PathRepertoirCourant(int partition, int RepertoirCourant);
void NomPathRecursive(int partition, int RepertoirCourant);
char *LectureFichierTexte();
void DiviseDataBloc(char *ContenuFichier);
int  RenommerElement(int partition, int RepertoirCourant, char *nom);
int  CollerElement(int partition, int RepertoirPere, int original_ID);
int  SplitLigneCommande(int partition, int RepertoirCourant, char decoupe[10][20], char commande[10], char argument[15] );
int  ChangerRepertoirCourant(int partition, int RepertoirCourant, char *nom);
int  CopierElement(int partition, int RepertoirParent, char *nom);
int  CollerDossier(int partition, int RepertoirPere, int original_ID);
int  CollerFichier(int partition, int RepertoirPere, int original_ID);
void Reboot(int partition);

#endif
