#include "fonctions.c"
/**
 * \file programme.c
 * \brief Fichier contenant le programme qui simule une interface shell via lquelle on peut
 * tester les fonctions implémentées sur la partition.
 * \author Agrane Amine 
 * \date 28 mars 2019
 */
int main(int argc, char ** argv) 
{
  //if (argc != 2) { erreur("Utilisation : ./creat <Nom Partition>)"); }
  char commande[10], argument[15];	
  char* reponse;
  char decoupe[10][20];
  int partition = open("partition", O_RDWR);
  char nom[MAXNOMFICHIER]; 
  int b=1;
  char nombloc[MAXNOMFICHIER];
  int nb_arguments;
  int RepertoirCourant =0;
  int Copie=-1;
  int CopierColler=1;
  int RepertoirCopie=-1;

    //reboot(partition);
	while(b>0)
	{
	 nb_arguments=SplitLigneCommande(partition, RepertoirCourant, decoupe,commande,argument);

	if (strcmp(commande, "dossier") == 0) 
    {
		 if(nb_arguments == 2 && strcmp(argument,"\n")!=0)			 
		 {CreationDossier(partition, RepertoirCourant, argument); }
		 else
		 {printf("Syntaxe : dossier <nom_dossier>\n");}
    } 
    else if (strcmp(commande, "supprimer") == 0)
    {	 
		 if(nb_arguments == 2 && strcmp(argument,"\n")!=0)
		 { SupprimmerElementRepertoir(partition, RepertoirCourant, argument); }
		 else
		 {printf("Syntaxe : delete <nom_element>\n");}		
    }
    else if (strcmp(commande, "ls") == 0)
    {
         AffichageContenuDossier(partition, RepertoirCourant);
    }		
    else if (strcmp(commande, "cd") == 0)
    {
		if(nb_arguments == 2 && strcmp(argument,"\n")!=0)	
		{RepertoirCourant = ChangerRepertoirCourant(partition, RepertoirCourant, argument);}
		else
		{printf("Syntaxe : cd <nom_dossier>\n");}		
    }
	else if (strcmp(commande, "contenu") == 0)
    {
		if(nb_arguments == 2 && strcmp(argument,"\n")!=0)
		{AffichageContenuFichier( partition, RepertoirCourant, argument);}
		else
		{printf("Syntaxe : content <nom_fichier>\n");}
    }
	else if (strcmp(commande, "copier") == 0)
    {
		if(nb_arguments == 2 && strcmp(argument,"\n")!=0)
		{
         Copie = CopierElement(partition, RepertoirCourant, argument);		 
		 CopierColler=1;
		}
		else
		{printf("Syntaxe : conpier <nom_element>\n");}
    }
	else if (strcmp(commande, "couper") == 0)
    {
		if(nb_arguments == 2 && strcmp(argument,"\n")!=0)
		{		
         Copie = CopierElement(partition, RepertoirCourant, argument);
		 RepertoirCopie= RepertoirCourant;
		 CopierColler=0;
		}
		else
		{printf("Syntaxe : couper <nom_element>\n");}		
    }
	else if (strcmp(commande, "coller") == 0)
    {    if(CopierColler==1)
  	      {CollerElement(partition, RepertoirCourant, Copie);}
	      else
		  {CouperColler(partition, RepertoirCourant, RepertoirCopie, Copie);}
    }	
    else if (strcmp(commande, "exit") == 0)
    {  
		b=-1; 
	}
	else if (strcmp(commande, "fichier") == 0)
    { 
		if(nb_arguments == 2 && strcmp(argument,"\n")!=0)
		{ CreationFichier(partition, RepertoirCourant, argument); }
		else
		{printf("Syntaxe : copier <nom_fichier>\n");}
	}
	else if (strcmp(commande, "renommer") == 0)
    {  
		if(nb_arguments == 2 && strcmp(argument,"\n")!=0)
		{RenommerElement(partition, RepertoirCourant, argument);}
		else
		{printf("Syntaxe : renommer <nom_element>\n");}
	}
	else if (strcmp(commande, "clear") == 0)
    {  
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); 
	}
	else if (strcmp(commande, "reboot") == 0)
    {  
		printf("Réinitialiser l'ensemble de la partition ? y/n\n"); 
		fgets(reponse,sizeof(reponse),stdin);
	    strtok(reponse, "\n");
		if(strcmp(reponse,"y") ==0)
		{reboot(partition);}
	}		
	else
	{
		printf("La commande '%s' n'est pas reconnue\n",commande);
	}
	
	}

  close(partition);
  return 0;
}
