#include "head.h"
/**
 * \file fonctions.c
 * \brief Fichier contenant l'ensemble des fonctions et procédures du projet.
 * \author Agrane Amine 
 * \version 1
 * \date 28 mars 2019
 */

                /******************* Fichier C de définitions des fonctions et procédures utilisées ****************/
												  
/**
 * \fn void LSEEK(int partition, int NumeroBloc)
 * \brief Cette fonction prend en argument le descripteur de fichier de la partition et un entier représentant l'identifiant
 *  unique d'un bloc à l'intérieur de la partition. La fonction calcule alors la "distance" entre le début de la partition et la position
 * du bloc, et déplace le pointeur de lecture/écriture sur cette position.
 * \param partition descripteur de fichier de la partition.
 * \param NumeroBloc identifiant unique du bloc à l'intérieur de la partition.
 */
void LSEEK(int partition, int NumeroBloc)
{	
	long OFF_SET = NumeroBloc*sizeof(Bloc);
	if ( OFF_SET != lseek(partition, NumeroBloc*sizeof(Bloc), SEEK_SET) )   { erreur("lseek erreur");}
}


/**
 * \fn void WRITE(int partition, int NumeroBloc, Bloc bloc)
 * \brief Cette fonction prend en argument le descripteur de fichier de la partition, un entier représentant l'identifiant unique d'un bloc, 
 *  et une instance de la structure Bloc nommé bloc. Elle effectue un appel à la fonction LSEEK, ce qui assure que le pointeur de lecture/ecriture 
 *  pointe sur la position du bloc dont l'identifiant est passé en argument. Elle passe alors à son écriture sur la partition.
 *  En résumé, elle écrit l'instance 'bloc' à la position 'NumerBloc' sur la partition.
 * \param partition descripteur de fichier de la partition.
 * \param NumeroBloc identifiant unique du bloc à l'intérieur de la partition.
 * \param bloc instance de la structure Bloc. 
 */
void WRITE(int partition, int NumeroBloc, Bloc bloc)
{
	LSEEK(partition, NumeroBloc);
	if (write(partition, &bloc,sizeof(Bloc)) != sizeof(Bloc))
		{ erreur("Write : erreur"); }
}


/**
 * \fn Bloc READ (int partition, int NumeroBloc)
 * \brief  Cette fonction prend en argument le descripteur de fichier de la partition et un entier représentant l'identifiant
 *  unique d'un bloc à l'intérieur de la partition. Elle effectue un appel à la fonction LSEEK, ce qui assure que le pointeur de lecture/ecriture 
 *  pointe sur la position du bloc dont l'identifiant est passé en argument. Elle lit alors le bloc se trouvant à cette position et le retourne.
 *  En résumé, elle lit le bloc dont l'identifiant unique est passé en argument et le retourne.
 * \param partition  descripteur de fichier de la partition.
 * \param NumeroBloc identifiant unique du bloc à l'intérieur de la partition.
 * \return bloc Valeur du bloc dont l'identifiant est passé en argument.
 */
Bloc READ (int partition, int NumeroBloc)
{
	if (NumeroBloc > NOMBREBLOCS) {erreur("READ Bloc hors de la partition"); }
	LSEEK(partition, NumeroBloc);
    Bloc bloc;
    if (sizeof(Bloc) != read(partition,&bloc,sizeof(Bloc))) { erreur("Read : erreur"); }
	return bloc;
}

/**
 * \fn static void erreur(char *message) 
 * \brief  Imprime le message d'erreur sur le flux standard stderr.
 * \param message contient le message d'erreur à afficher sur la sortie standar stderr.
 */
static void erreur(char *message) 
{
  int errnum;
  fprintf(stderr,"Erreur : %s\n",message);
  //fprintf(stderr, "%s\n", strerror( errnum ));
  exit(1);
}

/**
 * \fn void AffichageBloc(Bloc bloc)
 * \brief   prend un bloc en argument, et affiche la valeur de ses différents attributs.
 * \param bloc instance de la structure Bloc. 
 */
void AffichageBloc(Bloc bloc)
{
   // Au début, on affiche les éléments en communs dans le cas ou le bloc représente un fichier ou un dossier
   printf("\nType : %s | ID: %d | %s | Nom bloc : %s | Bloc suivant : %d\n", 
	 (bloc.FICHIER_DOSSIER?"Fichier":"Dossier"),bloc.ID_BLOC, (bloc.LIBRE?"Libre":"Occupé"), bloc.NOM_BLOC, bloc.BLOC_SUIVANT);
	
	if(bloc.FICHIER_DOSSIER) // Si le bloc représente un fichier, on affiche son contenu, la taille de son contenu ainsi que son espace libre disponible
	     {  printf("Taille max data %ld\t Taille réelle data %ld\t | Espace restant sur ce bloc %ld\n",   
				             MAXDONNEEBLOC, strlen(bloc.DONNEES), (MAXDONNEEBLOC-strlen(bloc.DONNEES))); 
		    printf("Data contenu dans ce fichier : %s", bloc.DONNEES);}
	//Dans le cas ou le bloc représente un dossier, on affiche son tableau de liens
	else { for(int i=0;i<15;i++) { printf("%d |", bloc.LIENS[i]);  } }
   printf("\n");  

}

/**
 * \fn void AffichagePartition(int partition)
 * \brief   Parcours et affiche l'ensemble des blocs de la partition.
 * \param partition descripteur de fichier de la partition.
 */
void AffichagePartition(int partition)
{
  Bloc bloc;
  for (int i = 0; i < NOMBREBLOCS; i++) 
  {  
    bloc=READ(partition,i);
    printf("Bloc Numéro %d :",i);   
	AffichageBloc(bloc);
  }
}

/**
 * \fn void AffichageContenuFichier(int partition, int RepertoirCourant, char *nom)
 * \brief  Affiche le contenu d'un FICHIER de la partition, dans le cas ou le fichier est stocké sur plusieurs blocs,
 on parcours l'ensemble des blocs et on affiche leurs contenus au fur et à mesur jusqu'a arriver au dernier bloc du fichier.
 * \param nom représente le nom du fichier dont on veut afficher le contenu
 * \param RepertoirCourant identifiant unique du bloc représentant le dossier qui contient le fichier dont on veut afficher le contenu.
 * \param partition descripteur de fichier de la partition.
*/
void AffichageContenuFichier(int partition, int RepertoirCourant, char *nom)
{
	int NumeroBloc;  
    int id_bloc= IdNomElement(partition, RepertoirCourant, nom);
    
	if(id_bloc != -1)
	{
	   do 
	    {
	    Bloc bloc=READ(partition, id_bloc);
	    printf("%s",bloc.DONNEES);
	    id_bloc=bloc.BLOC_SUIVANT;
	    } while( id_bloc !=-1 );
	}
}

/**
 * \fn void AffichageContenuDossier(int partition, int NumeroBloc)
 * \brief   Affiche le contenu d'un REPERTOIR dont l'identifiant est passé en argument.
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant unique du repertoir dont on veut afficher le contenu.
*/
void AffichageContenuDossier(int partition, int RepertoirCourant)
{
    Bloc bloc=READ(partition, RepertoirCourant);
	Bloc b;
	int i=0, vide=1;
	while(i < 15)
	{ 
		// On parcours le tableau de liens du bloc représentant le repertoir
		// Pour chaque id != -1, on lit le bloc correspondant et on affiche son nom
		// Les fichier sont entourés de ' ' tandis que les dossiers sont entourés de  " "
		if(bloc.LIENS[i] != -1)
		{
		 b=READ(partition, bloc.LIENS[i]);
			if(b.FICHIER_DOSSIER)
			{ printf("'%s'    ",b.NOM_BLOC);} //Fichier
			else
			{ printf("\"%s\"    ",b.NOM_BLOC);}	//Dossier			
		 vide=0;
		}
	i++;
	
	}
    if(vide) {printf("Le dossier '%s' est vide !\n",bloc.NOM_BLOC);}
	else{printf("\n");}
}

/**
 * \fn int PremierBlocVide(int partition)
 * \brief   Retourne l'identifiant du premier bloc libre dans la partition (Ordre descendant), si tout les blocs occupés, retourne -1.
 * \param partition descripteur de fichier de la partition.
 * \return identifiant du premier bloc vide à l'intérieur de la partition.
*/
int PremierBlocVide(int partition)
{
  Bloc bloc; int i=0; 
	
  while ( i <= NOMBREBLOCS) 
  {
    LSEEK(partition, i);
    if (sizeof(struct Bloc) != read(partition,&bloc,sizeof(Bloc)))  { erreur("PremierBLocVide : read erreur"); }
	  if(bloc.LIBRE) { return i; }
	  else  { i++;  }
  
  }
	erreur("PremierBlocVide : Mémoire pleine !\n"); 
}

/**
 * \fn Bloc InitBloc(int fd, int id, int libre, int suiv, char nom[MAXNOMFICHIER], char *donnees, int pere)
 * \brief   Retourne un bloc initialisé avec les valeurs passées en arguments. Constitue en quelque sorte le contructeur de la structure Bloc.
 * \param fd Indique si le bloc représente un fichier ou un dossier (1 => fichier 0 => dossier).
 * \param id Identifiant unique du bloc à l'intérieur de la partition.
 * \param libre Indique si le bloc est occupé ou non (1 => Libre 0 => occupé).
 * \param suiv Contient l'identifiant du bloc qui contient la suite du fichier prend -1 si le fichier est contenu dans un seul bloc 
 *  ou si le bloc représente un dossier.
 * \param nom Le nom du Bloc (Nom du fichier ou du dossier).
 * \param donnees Les données stockées sur le bloc(Contenu d'un fichier).
 * \param pere  L'identifiant unique du répertoir Parent d'un fichier ou d'un dossier.
 * \return bloc une instance de la structure Bloc initialisé avec les valeurs passées en arguments.
*/
Bloc InitBloc(int fd, int id, int libre, int suiv, char nom[MAXNOMFICHIER], char *donnees, int pere)
{
 Bloc bloc;
 bloc.FICHIER_DOSSIER =fd ;  
 bloc.ID_BLOC=id; 
 bloc.LIBRE=libre; 
 bloc.BLOC_SUIVANT=suiv; 
 strcpy(bloc.NOM_BLOC, nom);
 strcpy(bloc.DONNEES, donnees); 
 memcpy(bloc.LIENS, tab_init, sizeof(tab_init));
 bloc.PERE = pere;
 return bloc;
}

/**
 * \fn void NomBloc(int partition, int NumeroBloc, char nombloc[MAXNOMFICHIER])
 * \brief   prend l'id du bloc, et met son nom à l'intérieur de nombloc.
 * \param partition descripteur de fichier de la partition.
 * \param[out]  nombloc argument en sortie qui contiendra après l'appel de la fonction le nom du bloc
    dont l'identifiant est passé en argument.
 * \param NumeroBloc identifiant unique du bloc à l'intérieur de la partition.
*/
void NomBloc(int partition, int NumeroBloc, char nombloc[MAXNOMFICHIER])
{
	Bloc bloc = READ (partition,NumeroBloc);
	strcpy( nombloc, bloc.NOM_BLOC);

}

/**
 * \fn int IdNomElement(int partition, int RepertoirParent, char nom[MAXNOMFICHIER])
 * \brief  Retourne l'identifiant qui correspond au nom d'un fichier/dossier à l'intérieur du répertoir courant.
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir contenant le fichier dont le nom est passé en argument.
 * \param nom représente le nom du fichier/dossier dont on veut récupérer l'identifiant
 * \return identifiant l'identifiant de l'élément dont le nom est passé en argument, retourne -1 si il n'existe 
    pas le répertoir courant
*/
int IdNomElement(int partition, int RepertoirCourant, char nom[MAXNOMFICHIER])
{
    char nombloc[MAXNOMFICHIER];
	Bloc parent=READ(partition, RepertoirCourant); Bloc bloc; int i=0;
	while(i < TAILLEREPERTOIR)
	{
	 if(parent.LIENS[i]!=-1) // C.A.D Un Fichier/Dossier qui se trouve à l'intérieur de Repertoir Parent
	 { 	
		 NomBloc(partition, parent.LIENS[i], nombloc);
		 if(!strcmp(nom,nombloc))
		 {  return parent.LIENS[i]; }
	 }
     i++;
	}
printf(" '%s' cet élément n'existe pas dans ce repertoir !\n", nom);
return -1;
}


/**
 * \fn int ElementExisteRepertoir(int partition, int RepertoirParent, char nom[MAXNOMFICHIER])
 * \brief  Vérifie si un fichier/dossier qui porte un certain nom existe ou non dans un répertoir.
 *  1=>existe  -1=> n'existe pas.
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir contenant le fichier dont le nom est passé en argument.
 * \param nom représente le nom du fichier/dossier dont on veut vérifier l'existence à l'intérieur du répertoir courant
*/
int ElementExisteRepertoir(int partition, int RepertoirCourant, char nom[MAXNOMFICHIER])
{
    char nombloc[MAXNOMFICHIER];
	Bloc parent=READ(partition, RepertoirCourant); Bloc bloc; int i=0;
	while(i< TAILLEREPERTOIR)
	{
	 if(parent.LIENS[i]!=-1) // C.A.D Un Fichier/Dossier qui se trouve à l'intérieur de Repertoir Parent
	 { 	
		 NomBloc(partition, parent.LIENS[i], nombloc);
		 if(!strcmp(nom,nombloc)) {  return 1; }
	 }
     i++;
	}
return -1;
}


/**
 * \fn int SplitLigneCommande(int partition, int RepertoirCourant, char decoupe[10][20], char commande[10], char argument[15] )
 * \brief  Prend la ligne de commande de l'utilisateur, la decoupe et la stocke dans 
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir contenant le fichier dont le nom est passé en argument.
 * \param nom représente le nom du fichier/dossier dont on veut vérifier l'existence à l'intérieur du répertoir courant
*/
// 
int SplitLigneCommande(int partition, int RepertoirCourant, char decoupe[10][20], char commande[10], char argument[15] )
{
	int n=0,i,j=0;
	char ligne[25];

	PathRepertoirCourant(partition,RepertoirCourant);
    fgets(ligne,sizeof(ligne),stdin); 
	
	for(i=0;1;i++)
	{
		if(ligne[i]!=' ')
		{
			decoupe[n][j++]=ligne[i];
		}
		else
		{
			decoupe[n][j++]='\0'; 
			n++; 
			j=0;
		}
		if(ligne[i]=='\0')
		    break;
	}
	n++;
	if(n==1)
	{
	strcpy(commande, decoupe[0]);
	strcpy(argument, "");
	strtok(commande, "\n");
	return 1;
	}
	else if(n==2 && decoupe[1]!="" && decoupe[1]!="\n")
	{
	strcpy(commande, decoupe[0]);
    strcpy(argument, decoupe[1]);
	strtok(argument, "\n");
	return 2;
	}

	else
	{return -1;}
}
/**
 * \fn int AjouterLien(int partition, int RepertoirParent, int lien) 
 * \brief  prend le parent et ajoute l'identifiant passé en argument dans son tableau LIENS
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirParent identifiant du repertoir auquel on veut ajouter un lien.
 * \param identifiant identifiant unique d'un fichier/dossier qui se trouve dans le RepertoiPere
*/
int AjouterLien(int partition, int RepertoirParent, int identifiant) 
{
	int i=0;
	Bloc parent = READ(partition,RepertoirParent);
    while(i< TAILLEREPERTOIR && parent.LIENS[i] != -1)
	  { i++; }
	
 	if(i< TAILLEREPERTOIR) 
	{ 
	parent.LIENS[i]=identifiant; 
	WRITE(partition,RepertoirParent, parent); 
	return 1; 
	}
	else { 	erreur("AjouterLien : Mémoire pleine !\n");  }
}

/**
 * \fn int SupprimmerLien(int partition, int RepertoirParent, int identifiant) 
 * \brief Supprimme identifiant du tableau LIENS du repertoir parent, retourne 1 si bien supprimmé -1 sinon
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirParent identifiant du repertoir auquel on veut supprimmer un lien.
 * \param identifiant identifiant unique d'un fichier/dossier qui se trouve dans le RepertoiPere
*/
int SupprimmerLien(int partition, int RepertoirParent, int identifiant) 
{
	int i=0;
    Bloc parent = READ(partition,RepertoirParent);
	while(i< TAILLEREPERTOIR && parent.LIENS[i] != identifiant)
	{ i++; }
	
 	if(i< TAILLEREPERTOIR)
	{parent.LIENS[i]=-1; WRITE(partition,RepertoirParent, parent); return 1;}
}

/**
 * \fn void SupprimmerElementRepertoir(int partition, int RepertoirParent, char *nom)
 * \brief Supprimme un élément (dossier ou fichier) d'un Répertoir.
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirParent identifiant du repertoir auquel on veut supprimmer un élément.
 * \param nom nom du fichier/dossier qu'on souhaite supprimmer du RepertoirParent
*/
void SupprimmerElementRepertoir(int partition, int RepertoirParent, char *nom)
{
  Bloc bloc;
  int id_bloc= IdNomElement(partition, RepertoirParent, nom);
		
	if(id_bloc!=-1)
	{
  bloc = READ(partition,id_bloc);
		
  if(bloc.FICHIER_DOSSIER) // Si l'élément est un fichier
   {  	SupprimmerFichier(partition, RepertoirParent, id_bloc); 	}
  else					   // Si c'est un dossier
   { SupprimmerDossier(partition, RepertoirParent, id_bloc);}
	}
}

/**
 * \fn void SupprimmerFichier(int partition,  int RepertoirCourant, int NumeroBloc)
 * \brief Supprimme le bloc dont l'identifiant est passé en argument. (Cas d'un fichier)
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir contenant le fichier dont le nom est passé en argument.
 * \param NUmeroBloc identifiant unique du bloc à supprimmer.
*/
void SupprimmerFichier(int partition,  int RepertoirCourant, int NumeroBloc)
{
	if (NumeroBloc > NOMBREBLOCS) {erreur("Supprimmer fichier Bloc hors de la partition"); }
	
	SupprimmerLien(partition, RepertoirCourant, NumeroBloc);
    Bloc bloc=READ(partition, NumeroBloc);
	int BLOCSUIVANT = bloc.BLOC_SUIVANT;
	bloc.LIBRE = 1; 
	bloc.BLOC_SUIVANT = -1;
	strcpy(bloc.NOM_BLOC, "");
    strcpy(bloc.DONNEES, ""); 
	memcpy(bloc.LIENS, tab_init, sizeof(tab_init));
	WRITE(partition, NumeroBloc, bloc);
	
	if(BLOCSUIVANT!=-1) // C'est à dire que c'est un fichier stocké sur plusieurs blocs
	   { SupprimmerFichier(partition, RepertoirCourant, BLOCSUIVANT);   }
}

/**
 * \fn void SupprimmerDossier(int partition,  int RepertoirParent, int NumeroBloc)
 * \brief Supprimme le bloc dont l'identifiant est passé en argument. (Cas d'un dossier)
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir contenant le fichier dont le nom est passé en argument.
 * \param NUmeroBloc identifiant unique du bloc à supprimmer.
*/
void SupprimmerDossier(int partition,  int RepertoirParent, int NumeroBloc)
{
	// On supprimme le lien du fichier du répertoir parent.
	SupprimmerLien(partition, RepertoirParent, NumeroBloc);
	int i=0; 
	Bloc dossier=READ(partition,NumeroBloc);
	Bloc bloc;
	
	// On suprrimme un à un le contenu du dossier selon que ce soit un fichier ou un autre dossier.
	// Ces appels récursives assurent de supprimmer l'arboresence des dossiers et de leurs contenus présent dans le répertoir parent.
	while(i< TAILLEREPERTOIR)
	{	
		if(dossier.LIENS[i]!=-1)
		 {
			bloc=READ(partition,dossier.LIENS[i]);
			if(bloc.FICHIER_DOSSIER)
			 { SupprimmerFichier(partition, dossier.ID_BLOC, bloc.ID_BLOC); }
		 	else
		 	 {SupprimmerDossier(partition, dossier.ID_BLOC, bloc.ID_BLOC); }	
		 }
	 i++;
	}

 // Arrivé ici le dossier est vidé de tout contenu, on le supprimme alors comme simple fichier
 SupprimmerFichier(partition,RepertoirParent,NumeroBloc);
}

/**
 * \fn int CreationFichier(int partition, int RepertoirCourant, char nom[MAXNOMFICHIER] )
 * \brief Créaton d'un fichier dont le nom est passé en argument. prend en considération le cas ou le fichier tient sur un eul bloc,
 / et le cas ou plusieurs blocs sont nécessaires afin de stocker le fichier.
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir qui contiendra le fichier crée.
 * \param nom du fichier à creer.
 * \return retourne 1 en cas de réussite, -1 sinon
*/
int CreationFichier(int partition, int RepertoirCourant, char nom[MAXNOMFICHIER] )
{
  char *ContenuFichier=LectureFichierTexte();
  int taille_fichier = strlen(ContenuFichier);
  int max = MAXDONNEEBLOC -20;
  int nbBloc= (taille_fichier / max)+1;
  Bloc bloc; 
 
  int id = PremierBlocVide(partition);
  int a=1;
	
  if(ElementExisteRepertoir(partition,RepertoirCourant, nom) == -1)
	{
      // Dans le cas ou un seul bloc suffit pour stocker le fichier
	  if(max>taille_fichier)
	  {
		   //Création du bloc, ecriture sur la partition, ajout du lien dans le répertoir parent
           bloc=InitBloc(1,id,0,-1,nom,ContenuFichier,RepertoirCourant);       
           WRITE(partition, id, bloc);
           AjouterLien(partition, RepertoirCourant, id); 
           return 1;
	  }
	  else
	  {    // On suit la meme logique ici, sauf qu'il y aura plusieur bloc liés, à par le dernier bloc qui ne pointe sur aucun autre.
		   int borneINF=0;
           char bloc_data[max];
	       strncpy ( bloc_data, ContenuFichier + borneINF, max );			 		   
		   bloc=InitBloc(1,id,0,id+1,nom,bloc_data,RepertoirCourant); 	 
		
           WRITE(partition, id, bloc);
           AjouterLien(partition, RepertoirCourant, id);
		   borneINF=borneINF+max+1; 
		  		// On découpe l'ensemble du contenu de fichier en intervalles de taille égales
		  	    // Chaque bloc contiendra une intervalle de données
                for(int i=1; i<nbBloc; i++)  // Pour chaque bloc
                { 
				  id=PremierBlocVide(partition);
                  strncpy ( bloc_data, ContenuFichier + borneINF,max );
				  bloc=InitBloc(1,id,0,id+1,"",bloc_data,-1);
				  WRITE(partition, id, bloc);
                  borneINF=borneINF+max+1; 
				   
                }     
            
		  bloc=READ(partition, id); 
		  bloc.BLOC_SUIVANT=-1; 
		  WRITE(partition, id, bloc);  
		  return 1;
	  }
    }
	
  else
	{erreur("Un élément dans le répertoir porte déjà ce nom !\n");}
}

/**
 * \fn int CreationDossier(int partition, int RepertoirParent, char *nom_dossier)
 * \brief Créaton d'un dossier dont le nom est passé en argument. 
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir qui contiendra le dossier crée.
 * \param nom du dossier à creer.
 * \return retourne 1 en cas de réussite, -1 sinon
*/
int CreationDossier(int partition, int RepertoirParent, char *nom_dossier)
{ 
//Création du bloc, ecriture sur la partition, ajout du lien dans le répertoir parent
    Bloc bloc; 
	if(ElementExisteRepertoir(partition,RepertoirParent, nom_dossier) == -1)
	{
  int id = PremierBlocVide(partition);
  bloc=InitBloc(0,id,0,-1,nom_dossier,"",RepertoirParent);       
  WRITE(partition, id, bloc);
  AjouterLien(partition, RepertoirParent, id); 
  return id;
	}
	
  else
	{printf("Un élément dans le répertoir porte déjà ce nom !\n"); return 0;}
}


/**
 * \fn int ChangerRepertoirCourant(int partition, int RepertoirCourant, char *nom)
 * \brief Fonction qui assure le changement de repertoir courant. Elle retourne l'identifiant du bloc
 * qui représente le dossier ou on veut se déplacer. prend le nom du dossier et retourne son identifiant qui sera placé comme
 * nouveau Repertoir Courant. Si le nom == "..", on remonte dans l'arborescene, en retournant l'identifiant du pere du repertoir courant.
 * Le repertoir Home est la racine de la partition et ne possede pas de repertoir pere.
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir courant actuelle.
 * \param nom du dossier auquel on veut se déplacer.
 * \return retourne l'identifiant du nouveau repertoir courant.
*/
int ChangerRepertoirCourant(int partition, int RepertoirCourant, char *nom)
{
   if (strcmp(nom, "..") == 0) 
   {
	  Bloc parent = READ(partition, RepertoirCourant);
	  if(parent.PERE == -1)
	  {printf("Nous somme à la racine de cette partition !\n"); return RepertoirCourant;}
	   else
	  {return parent.PERE;}
   }
   else
   {
	   // Identifiant unique du dossier dont le nom est passé en pramètres
	   // Le dossier doit se trouver dans le Repertoir courant actuel 
	  int id_bloc= IdNomElement(partition, RepertoirCourant, nom);
	   if(id_bloc!=-1)
	   {
         Bloc bloc = READ(partition, id_bloc);
	     if(bloc.FICHIER_DOSSIER)
		 {printf(" '%s' est un fichier pas un dossier !\n",nom); return RepertoirCourant;}
		 else
		 {return bloc.ID_BLOC;}
	   }
	   else
	   {
	     return RepertoirCourant;
	   
	   }
	} 
}

/**
 * \fn void PathRepertoirCourant(int partition, int RepertoirCourant)
 * \brief Fonction qui affiche le repertoir courant actuel. On utilise des appels récursive afin d'afficher
 * le "path" du repertoir courant à partir du repertoir racine "Home".
 * par exemple : /Home/Dossier1/Dossier2>
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir courant actuelle.
*/
void PathRepertoirCourant(int partition, int RepertoirCourant)
{
printf("user:/Home");
Bloc bloc=READ(partition, RepertoirCourant);
if(bloc.PERE !=-1)
{   NomPathRecursive(partition, bloc.PERE);
	printf("/%s",bloc.NOM_BLOC);
}
printf(">");

}

/**
 * \fn void NomPathRecursive(int partition, int RepertoirCourant)
 * \brief Fonction récursive qui permet d'afficher le path pour arriver au répertoir actuel en démarrant de la racine.
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir courant actuelle.
*/
void NomPathRecursive(int partition, int RepertoirCourant)
{
Bloc bloc=READ(partition, RepertoirCourant);
if(bloc.PERE !=-1)
{   NomPathRecursive(partition, bloc.PERE);
	printf("/%s",bloc.NOM_BLOC);
}

}

/**
 * \fn int CopierElement(int partition, int RepertoirParent, char *nom)
 * \brief Fonction qui retourne l'identifiant unique du fichier/dossier se trouvant dans le repertoir courant 
 * qu'on souhaite copier et dont le nom est passé en argument.
 * La fonction retourne l'identifiant de l'élément afin qu'il soit stocké en mémoire
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir courant actuelle.
 * \return identifiant l'identifiant unique du fichier/dossier que l'on souhaite copier, sinon -1 si il 
 n'existe pas dans le repertoir courant.
*/
int CopierElement(int partition, int RepertoirParent, char *nom)
{
  int id= IdNomElement(partition, RepertoirParent, nom);
  if(id==-1)
  {
	  return -1;
  }
  else
  { 
	  printf("Elément copié !\n");
	  return id;
  }
}

/**
 * \fn int CollerElement(int partition, int RepertoirPere, int original_ID)
 * \brief Fonction qui colle une copie à l'intérieur du repertoir courant du fichier/dossier dont l'id est passé en argument 
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir courant actuelle.
 * \param original_ID identifiant unique de l'élément r que l'on souhaite coller à l'intérieur de notre
 * repertoir courant.
 * \return identifiant retourne l'identifiant du nouveau bloc représentant la copie du fichier/dossier, -1 si erreurs.
*/
int CollerElement(int partition, int RepertoirPere, int original_ID)
{
	char nom[MAXNOMFICHIER];
	NomBloc(partition, original_ID, nom);
	int id;
	
  if(original_ID == -1)
	{
	  printf("Aucun fichier copier en attente ! \n"); 
	  return -1;
    }
  else
  {
	if(ElementExisteRepertoir(partition,RepertoirPere, nom) == -1)  
	{
       Bloc original= READ(partition, original_ID);
      // printf("L'élément collé dans ce répertoir est le %s : %s\n",original.FICHIER_DOSSIER?"Fichier":"Dossier", original.NOM_BLOC);
	  // SI l'élément à coller est un fichier
      if(original.FICHIER_DOSSIER)
      {
	  id=CollerFichier(partition, RepertoirPere, original_ID);
      }
	  // Si l'élément à coller est un dossier
      else
      {
	  id=CollerDossier(partition, RepertoirPere, original_ID);
      }
	}
	else 
	{printf("Un élément dans le répertoir porte déjà ce nom !\n"); return -1;}
	
  return id;
  }

}

/**
 * \fn int CollerDossier(int partition, int RepertoirPere, int original_ID)
 * \brief prend un RepertoirPere (son identifiant) et un dossier (son identifiant).
  Crée la copie du dossier orignal et le met dans Repertoir Pere.
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirPere identifiant du repertoir pere auquel on souhite coller le dossier.
 * \param original_ID identifiant unique du dossier que l'on souhaite coller à l'intérieur de notre
 * repertoir pere.
 * \return identifiant retourne l'identifiant du bloc contenant la copie du dossier passé en argument.
*/
int CollerDossier(int partition, int RepertoirPere, int original_ID)
{
// Pour le bloc du dossier, c'est un copie conforme, mis à par le tableau de LIENS.
// On doit  aussi effectuer la création d'une copie de tout les fichiers/dossier présent dans le repertoir
// On utilise pour ça des appels récursifs afin d'assurer la création de tout les enfants, petit enfants, etc, 
// descendants du repertoir pere
Bloc bloc_enfant;
Bloc copie_enfant;
int enfant;
int i=0;
int id =PremierBlocVide(partition);
Bloc original=READ(partition, original_ID);
Bloc copie;
AjouterLien(partition, RepertoirPere,  id);
copie.FICHIER_DOSSIER= original.FICHIER_DOSSIER;
copie.ID_BLOC=id;
copie.LIBRE=0;
copie.BLOC_SUIVANT=-1;
copie.PERE=RepertoirPere;
memcpy(copie.LIENS, tab_init, sizeof(tab_init));
strcpy(copie.NOM_BLOC,original.NOM_BLOC);
strcpy(copie.DONNEES, original.DONNEES);
WRITE(partition, id, copie);
 while(i < TAILLEREPERTOIR)
 {
	 // Si l'original contient un élément (fichier ou dossier) on doit en créer une copieret le mettre dans la copie du repertoir pere
    if(original.LIENS[i]!=-1)
	{	
		// On lit l'enfant à copier
		copie_enfant= READ(partition, original.LIENS[i]);
		
		// On colle la copier de l'enfant dans la copie du repertoir pere
		enfant = CollerElement(partition, id, original.LIENS[i]);
		bloc_enfant = READ(partition, enfant); 
		bloc_enfant.PERE=id;
		if(enfant != copie.ID_BLOC)
		  {copie.LIENS[i]=enfant;}
	}
	 i++;
 }

WRITE(partition, id, copie);

return id;
}


/**
 * \fn Bloc CopieConformeBloc(int partition, Bloc original)
 * \brief Retourne une copie conforme du dossier passé en argument
 * \param partition descripteur de fichier de la partition.
 * \param original instance de la structure Bloc dont on retourne une copie identique.
 * \return copie instance de la structure Bloc, et copie conforme du bloc passé en argument.
*/
Bloc CopieConformeBloc(int partition, Bloc original)
{
	Bloc copie;
	copie.ID_BLOC=PremierBlocVide(partition);
	copie.FICHIER_DOSSIER= original.FICHIER_DOSSIER;
    copie.LIBRE=0;
    copie.BLOC_SUIVANT=-1; // SUIVANT
    copie.PERE=-1;         // PERE 
    memcpy(copie.LIENS, tab_init, sizeof(tab_init));
    strcpy(copie.NOM_BLOC,original.NOM_BLOC);
    strcpy(copie.DONNEES, original.DONNEES);
	return copie;

}


/**
 * \fn int CollerFichier(int partition, int RepertoirPere, int original_ID)
 * \brief prend un RepertoirPere (son identifiant) et un fichier (son identifiant).
  Crée la copie du fichier orignal et le met dans Repertoir Pere. Traite du cas ou le fichier tient sur un bloc
 * et du cas ou le fichier est divisé sur plusieurs blocs
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirPere identifiant du repertoir pere auquel on souhite coller le fichier.
 * \param original_ID identifiant unique du fichier que l'on souhaite coller à l'intérieur de notre
 * repertoir pere.
 * \return identifiant retourne l'identifiant du bloc contenant la copie du fichier passé en argument.
*/
int CollerFichier(int partition, int RepertoirPere, int original_ID)
{
int id_copie=0;
Bloc original=READ(partition, original_ID);
Bloc copie=CopieConformeBloc(partition, original);
int first_id = copie.ID_BLOC;
copie.PERE=RepertoirPere;
AjouterLien(partition, RepertoirPere,  copie.ID_BLOC);
WRITE(partition, copie.ID_BLOC, copie); 

	// Cas ou le fichier à copier est un seul bloc	
	// On crée une copie de ce seul et unique bloc
	if(original.BLOC_SUIVANT==-1)
	{   
		copie.BLOC_SUIVANT=-1;
		WRITE(partition, copie.ID_BLOC, copie); 
		return first_id;
	}
	// Cas ou le fichier à copier est contenu sur plusieurs blocs
	// On doit procéder à la création des copies de chacun des blocs
	else
	{
		// Tant qu'il y a un bloc suivant
		while(original.BLOC_SUIVANT != -1)
		{  			    
			copie.BLOC_SUIVANT=copie.ID_BLOC+1;
			WRITE(partition, copie.ID_BLOC, copie); 
			
			// On recommence 
            original=READ(partition, original.BLOC_SUIVANT);  
			copie=CopieConformeBloc(partition, original);
			copie.PERE=-1;
			
			if(original.BLOC_SUIVANT==-1)
	        {   
		    copie.BLOC_SUIVANT=-1;
		    WRITE(partition, copie.ID_BLOC, copie); 
		     return first_id;
	        }					
		}			
	}
return first_id;
}




/**
 * \fn int CouperColler(int partition, int RepertoirCourant, int RepertoirCopie, int original_ID)
 * \brief Colle une copie  l'intérieur du repertoir courant du fichier/dossier copié précedemment, et supprimme 
 * l'élément original du repertoir dont il a été coupé.
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir courant actuelle.
 * \param original_ID identifiant unique de l'élément  que l'on souhaite coller à l'intérieur de notre repertoir courant
 * \return entier 1 si l'opération est un succes, -1 sinon.
*/
int CouperColler(int partition, int RepertoirCourant, int RepertoirCopie, int original_ID)
{
CollerElement(partition, RepertoirCourant, original_ID);
Bloc bloc = READ(partition,original_ID);
SupprimmerLien(partition, RepertoirCopie, original_ID);
		
  if(bloc.FICHIER_DOSSIER)
   {  	SupprimmerFichier(partition, RepertoirCourant, original_ID); 	}
  else
   { SupprimmerDossier(partition, RepertoirCourant, original_ID);}

}


/**
 * \fn int RenommerElement(int partition, int RepertoirCourant, char *nom)
 * \brief Renomme l'élément dont le nom est passé en argument.
 * \param partition descripteur de fichier de la partition.
 * \param RepertoirCourant identifiant du repertoir courant actuelle.
 * \param nom nom du fichier/dossier se trouvant dans le repertoir courant et que l'on souhaite renommer.
 * \return entier 1 si l'opération est un succes, -1 sinon.
*/
int RenommerElement(int partition, int RepertoirCourant, char *nom)
{
	char new[MAXNOMFICHIER] ;
  int id= IdNomElement(partition, RepertoirCourant, nom);
	if(id!=-1)
	{
      printf("Donner le nouveau nom de '%s':   ",nom);
      fgets(new,sizeof(new),stdin);
	  strtok(new, "\n");
	  Bloc bloc=READ(partition, id);
	  strcpy(bloc.NOM_BLOC,new);
	  WRITE(partition, id, bloc);
	  return 1;
	}
return -1;
}



/**
 * \fn char* LectureFichierTexte()
 * \brief Fonction qui lit le fichier texte "EcritureFichierTexte.txt", et retourne son contenu via 
 * un pointeur sur un tableau de char. Le contenu du fichier texte sera utilisé comme contenu du prochain fichier
 * créé à l'intérieur de notre partition
 */
char* LectureFichierTexte()
{
char *buffer,*ContenuFichier; long taille;
FILE *fp = fopen ( "EcritureFichierTexte.txt" , "r" );
if( !fp ) {perror("Erreur ouverture fichier EcrireFichier.txt"),exit(1);}

// On positionne le pointeur sur la fin du fichier
// Et on récupère la taille du fichier à l'aide de ftell()
fseek( fp , 0L , SEEK_END);
taille = ftell( fp );
rewind( fp );

/* Allocation de la mémoire pour la variable buffer*/
buffer = calloc( 1, taille+1 );
ContenuFichier = calloc(1, taille+1);
if( !buffer ) {fclose(fp),fputs("memory alloc fails",stderr),exit(1);}

/* On copie le contenu du fichier texte dans la variable buffer */
if( 1!=fread( buffer , taille, 1 , fp) )
  fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

/* On copie buffer dans la variable ContenuFichier*/
strcpy(ContenuFichier, buffer);

fclose(fp);
free(buffer);
return ContenuFichier;
}



/**
 * \fn void reboot(int partition) 
 * \brief Fonction qui "reboot" et initialise notre partition, en initialisant 1 à 1 chacun des blocs de la partition.
 * \param partition descripteur de fichier de la partition.
 */
void Reboot(int partition)
{
	//bloc racine en premier
	Bloc bloc=InitBloc(0,0,0,-1,"Home","",-1);      
	WRITE(partition, 0, bloc);

  for (int i = 1; i < NOMBREBLOCS; i++) 
  {
    bloc=InitBloc(-1,i,1,-1,"","",-1);
    WRITE(partition, i, bloc);
  }
}
