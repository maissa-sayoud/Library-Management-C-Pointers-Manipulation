/**************************************************************************************/
//                 _________________________________________________

/*________________  G E S T I O N    D E    B I B L I O T H E Q U E  _________________*/

//                 _________________________________________________

/**************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

//_____________________________________________________________________________________________________________________________________________________________________

//......................  TYPES   ...........................................................................................................................................................
//_____________________________________________________________________________________________________________________________________________________________________

//   LISTE des OUVRAGES :
typedef struct OuvrageListe* OListe;
typedef struct OuvrageListe
{
        char Cote [15];   //type: alpha-numerique(14 caracteres SANS ESPACE +'\0') /indique la localisation précise d'1 ouvrage.
        char Titre [100]; //il existe un livre avec 96 caracteres !
        int n_exmplr_biblio;  //introduit par l'utilisateur
        int n_exmplr_emprnt; //initialement à 0

        int n_emprunt;   //Bonus: initialement à 0; c le nombre de fois cet ouvrage a été emprunté

        OListe suiv;
} Ouvrage;

//   LISTE des ETUDIANTS :
typedef struct EtudListe* EListe;
typedef struct EtudListe
{
        char Matricule [13]; // 191 931 040 670 -> 12 cases + \0
        char Abonnement; //'P':premium ou 'C':classique
        int Penalite;  //initialement à 0

        int il_a_n_ouvrage; //Bonus: initialement à 0; il a emprunté n ouvrages_non_retournés

        EListe suiv;
} Etudiant;

//   LISTE des EMPRUNTS :
struct Date
{
        int jour;
        int mois;
        int an;
};
typedef struct EmpruntListe* PListe;
typedef struct EmpruntListe
{
        char Matricule [13];
        char Cote [15];
        struct Date Date_Debut;
        struct Date Date_Fin;

        PListe suiv;
} Emprunt;

//   LISTE des PENALITES :




//  La File :
typedef int Telm;

typedef struct Element* EFile;
typedef struct Element
{
        Telm  Val;
        EFile Suiv
} CelluleF;
typedef struct
{
        EFile Tete,Queue;
} File;

//_____________________________________________________________________________________________________________________________________________________________________

//......................  VARIABLES   ...........................................................................................................................................................
//_____________________________________________________________________________________________________________________________________________________________________

// variables globales :
int choix;
int N_Ouvrage, N_Etud;
int creer_O=0, Creer_E=0, creer_P=0;
char cote_Ouvrage [15], Matricule [13];

const Max_Penalite=7;

int Duree_Emprunt=0, Max_Ouvrage_Emprunte=0;

//Pointeurs des listes :
OListe TO; //Liste des Ouvrages.
EListe TE; //Liste des Etudiants.
PListe TP; //Liste des Emprunts.

//Pointeurs des Files :
File FP, FC;
struct attente
{
        char Matricule [13];
        char Cote [15]
};

//Extra:
// variables de la date
int day, mois, an;
time_t now;

//  variable Sleep
const tps=100;

//_________________________________________________________________________________________________________________________________________________

//..............     FONCTIONS & PROCEDURES    ..........................................................................................................................
//_________________________________________________________________________________________________________________________________________________

//   nettoyer le buffer: utiliser le sous prgrm a la placce du commentaire //clear();
/*int c;
while ((c = getchar()) != '\n' && c != EOF)
{ }*/

//   listes chainées:
//strcmp(str1,str2); retourne 0 if equal(other stuff if not)

void AfficherActions (void)
{
        printf ("\n\t\t 1>> Ajouter des ouvrages.\n");
        Sleep (tps);
        printf ("\t\t 2>> Consulter la liste de tous les ouvrages.\n");
        Sleep (tps);
        printf ("\t\t 3>> Supprimer un ouvrage.\n");
        Sleep (tps);
        printf ("\t\t 4>> V%crifier la disponibilit%c des exemplaires.\n",130,130);
        Sleep (tps);

        printf ("\n\t\t 5>> Ajouter des etudiants.\n");
        Sleep (tps);
        printf ("\t\t 6>> Consulter un membre.\n");
        Sleep (tps);
        printf ("\t\t 7>> Supprimer un membre.\n");
        Sleep (tps);
        printf ("\t\t 8>> Modifier un membre.\n");
        Sleep (tps);

        printf ("\n\t\t 9>> Ajouter des emprunts.\n");
        Sleep (tps);
        printf ("\t\t 10>> Consulter un emprunt.\n");
        Sleep (tps);
        printf ("\t\t 11>> Supprimer un emprunt.\n");
        Sleep (tps);
        printf ("\t\t 12>> Modifier un emprunt.\n");
        Sleep (tps);
        printf ("\t\t 13>> Retourner un emprunt.\n");
        Sleep (tps);

        printf ("\n\t\t 14>> Ajouter une p%cnalit%c.\n",130,130);
        Sleep (tps);
        printf ("\t\t 15>> Supprimer une p%cnalit%c.\n",130,130);
        Sleep (tps);
        printf ("\t\t 16>> Consulter la liste de p%cnalit%c.\n",130,130);
        Sleep (tps);

        printf ("\n\t\t 17>> D%cfinir la dur%ce des emprunts.\n",130,130);
        Sleep (tps);
        printf ("\t\t 18>> D%cfinir le MAX des ouvrages %c emprunter.\n",130,133);
        Sleep (tps);

        printf ("\n\t\t 19>> Consulter les ouvrages les plus sollicit%cs.\n",130);
        Sleep (tps);
        printf ("\t\t 20>> Consulter l'ouvrage le moins emprunt%c.\n",130);
        Sleep (tps);

        printf ("\n\t\t 0>> Quitter.\n");
        Sleep (tps);
}
//_________________________________________________________________________________________________________________________________________________
//**** OUVRAGES ****************************************************************************************************************************************

void CreerListeOuvrage(OListe *TO, int N_Ouvrage)
{
        OListe P,Q;
        int i=1;

        *TO=NULL;

        if (N_Ouvrage!=0)
        {
                P=malloc(sizeof(Ouvrage));
                printf ("\n\t Ouvrage %d :",i);

                printf ("\n\t\t La cote ne contient pas d'espace !\n\t\t Cote= ");
                scanf ("%s", &P->Cote);

                printf ("\n\t\t Titre= ");
                fflush(stdin); //clear the buffer !
                //clear(); //nettoyer le buffer si fflush ne marche pas !
                gets(P->Titre);

                printf ("\n\t\t N d'exemplaire dans la bib= ");
                scanf("%d", &P->n_exmplr_biblio);
                /*while(P->n_exmplr_biblio < 0);
                {
                        Beep(1000,500);
                        printf("ERREUR: Valeur invalide!\n\t\t N d'exemplaire dans la bib= ");
                        scanf("%d", &P->n_exmplr_biblio);
                }*/

                P->n_exmplr_emprnt=0;
                printf ("\n\t\t N d’exemplaire emprunt%c= %d",130,P->n_exmplr_emprnt);

                //Bonus :
                P->n_emprunt=0;

                *TO=P;
                Q=P;
                for (i=2; i<=N_Ouvrage; i++)
                {
                        P=malloc(sizeof(Ouvrage));
                        printf ("\n\t Ouvrage %d :",i);

                        printf ("\n\t\t Cote= ");
                        scanf ("%s", &P->Cote);

                        printf ("\n\t\t Titre= ");
                        fflush(stdin); //nettoyer le buffer
                        //clear(); //nettoyer le buffer si fflush ne marche pas !
                        gets (P->Titre);

                        printf ("\n\t\t N d’exemplaire dans la bib= ");
                        scanf("%d", &P->n_exmplr_biblio);
                        /*while(P->n_exmplr_biblio < 0);
                        {
                                Beep(1000,500);
                                printf("ERREUR: Valeur invalide!\n\t\t N d'exemplaire dans la bib= ");
                                scanf("%d", &P->n_exmplr_biblio);
                        }*/

                        P->n_exmplr_emprnt=0;
                        printf ("\n\t\t N d’exemplaire emprunt%c= %d",130,P->n_exmplr_emprnt);

                        //Bonus:
                        P->n_emprunt=0;

                        Q->suiv=P;
                        Q=P;
                }
                Q->suiv=NULL;
        }
}

void AjouterOuvrage (OListe *TO, int i)
{
        OListe Q,P;
        //printf("(*TO)=%d *T=%d TO=%d P=%d Q=%d",,TO,P,Q);

        P=malloc(sizeof(Ouvrage));
        printf ("\n\t NOUVEL Ouvrage %d :",i);

        printf ("\n\t\t Cote= ");
        scanf ("%s", &P->Cote);

        printf ("\n\t\t Titre= ");
        fflush (stdin);
        //clear(); //nettoyer le buffer si fflush ne marche pas !
        gets(P->Titre);

        printf ("\n\t\t N d’exemplaire dans la bib= ");
        scanf("%d", &P->n_exmplr_biblio);
        /*while(P->n_exmplr_biblio < 0);
        {
                Beep(1000,500);
                printf("ERREUR: Valeur invalide!\n\t\t N d'exemplaire dans la bib= ");
                scanf("%d", &P->n_exmplr_biblio);
        }*/

        P->n_exmplr_emprnt=0;
        printf ("\n\t\t N d’exemplaire emprunt%c= %d",130,P->n_exmplr_emprnt);

        //Bonus :
        P->n_emprunt=0;

        P->suiv=NULL;

        Q=*TO; // *TO // (*TO) ???

        while (Q->suiv!=NULL)
        {
                Q=Q->suiv;
        }
        Q->suiv=P;
}

void AfficherListeOuvrages (OListe TO)
{
        int i=1;
        if (TO==NULL)
        {
                printf("\n\n Erreur: La liste des ouvrages est VIDE !\n");
                Beep(1000,500);
                Sleep(3*tps);
        }
        else
        {
                while (TO!=NULL)
                {
                        printf ("\n\n\t Ouvrage %d :",i);

                        printf ("\n\t\t - Cote: %s",TO->Cote);
                        printf ("\n\n\t\t - Titre:");
                        puts(TO->Titre);
                        printf ("\n\t\t - Nombre d'exemplaires dans la bib: %d",TO->n_exmplr_biblio);
                        printf ("\n\n\t\t - Nombre d'exemplaires emprunt%cs: %d",130,TO->n_exmplr_emprnt);

                        TO=TO->suiv;
                        i++;
                        Sleep(5*tps);
                }
        }
}

/*int ChercherPositionOuvrage (OListe TO, char cote_Ouvrage [15], )
{
        trouv=0;
        while (TO!=NULL && trouv==0)
        {
                if (strcmp(TO->Cote,cote_Ouvrage)==1)
                        return(k);
                TO=TO->suiv;
        }
        if (TO==NULL) //la valeur n'existe pas !
                return (-1);
}*/

int ExisteCote(OListe TO, char cote_Ouvrage [15])
{
        if (TO!=NULL)
        {
                while(TO!=NULL)
                {
                        if (strcmp(TO->Cote,cote_Ouvrage)==0)
                                return 1;
                        TO=TO->suiv;
                }
        }
        return (0);
}

void SupprimerOuvrage (OListe *TO, char cote_Ouvrage[15], int *creer_O) //NON COMPLETE !!!!!!
{
        OListe P,Q;
        int sup =0;

        P=*TO;
        if (ExisteCote(P,cote_Ouvrage)==1)
        {
                //si c'est la tete => suppression au debut:
                P=*TO;
                //si l'element a supprimer est la tête :
                if(strcmp(P->Cote,cote_Ouvrage)==0)
                {
                        *TO=P->suiv;
                        free(P);
                        sup=1;
                        printf("\n L'ouvrage (cote= %s) a %ct%c supprim%c avec succ%cs\n",cote_Ouvrage,130,130,130,130);
                }
                else
                {
                        Q=*TO;
                        P=Q->suiv;
                        //suppression au milieu :
                        while(P->suiv!=NULL && sup==0)
                        {
                                if (strcmp(P->Cote,cote_Ouvrage)==0)
                                {
                                        Q->suiv=P->suiv;
                                        free(P);
                                        sup=1;
                                        printf("\nl'ouvrage (cote= %s) a %ct%c supprim%c avec succ%cs\n",cote_Ouvrage,130,130,130,130);
                                        sup=1;
                                }
                                P=P->suiv;
                                Q=Q->suiv;
                        }
                        //suppression a la fin:
                        if (P->suiv==NULL && strcmp(P->Cote,cote_Ouvrage)==0)
                        {
                                Q->suiv=NULL;
                                free (P);
                                printf("\nL'ouvrage (cote= %s) supprim%c avec succ%cs\n",cote_Ouvrage,130,130);
                                sup=1;
                        }
                }
        }

        //la liste est vide ou l'ouvrage n'existe pas:
        if (sup==0)
        {
                Beep(1000,800);
                printf("\n Cet Ouvrage (cote= %s) n'existe pas !!\n",cote_Ouvrage);
                Sleep(3*tps);
        }

        //pour ne pas avoir des problèmes lors de l'ajout d'un ouvrage après !
        if (*TO==NULL)
                creer_O=0;
}

int ExemplaireDispo (OListe TO, char cote_Ouvrage[15])
{
        OListe P;

        if (ExisteCote(TO,cote_Ouvrage)==0)
        {
                //ouvrage n'existe pas:
                Beep(1000,500);
                //printf("P->cote= %s et Cote_ouvrage= %s",P->Cote,cote_Ouvrage);
                printf ("\nOuvrage n'existe pas ou la cote=%s est incorrecte.\n\n",cote_Ouvrage);
                return 0;
        }
        else
        {
                //si l'ouvrage existe --> VERIFIER LA DISPO après l'avoir trouver :
                if (TO!=NULL)
                {
                        P=TO;
                        //printf("debut: P->cote= %s et Cote_ouvrage= %s",P->Cote,cote_Ouvrage);

                        while(P!=NULL && strcmp(P->Cote,cote_Ouvrage)!=0) //different de 0 = non egales !
                        {
                                //printf("boucle while : P->cote= %s et Cote_ouvrage= %s",P->Cote,cote_Ouvrage);
                                P=P->suiv;
                        }
                        //on est sorti de la boucle et P pointe l'élément qu'on désire verifier sa dispo !
                        if (P->n_exmplr_biblio>0)
                        {
                                //printf("\n\t\t OUI, L'ouvrage (cote= %s) est disponible !",cote_Ouvrage);
                                return 1;
                        }
                        else
                        {
                                //printf("\n\t\t L'ouvrage (cote= %s) n'est pas disponible !",cote_Ouvrage);
                                return 0;
                        }
                }
        }
}


//_________________________________________________________________________________________________________________________________________________

//**** ETUDIANTS ***************************************************************************************************************************************

void CreerListeEtud (EListe *TE, int N_Etud)
{
        EListe P,Q;
        int i=1;

        *TE=NULL;

        if (N_Etud!=0) //si la valeur entrée par l'utilisater est valide:
        {
                P=malloc(sizeof(Etudiant));
                printf ("\n\t Etudiant %d :",i);

                printf ("\n\t\t Matricule= ");
                scanf ("%s", &P->Matricule);
                fflush (stdin);

                printf ("\n\t\t Abonnement ('C': Classique ou 'P': Premium <-Majuscule->)=  ");
                do
                        scanf("%c",&P->Abonnement);
                while (P->Abonnement!='C' && P->Abonnement!='P');

                printf ("\n\t\t p%cnalit%c = 0 \n(par defaut, vous pouvez le changer dans la liste des actions).",130,130);
                P->Penalite=0; //selon l'ennoncé la pénalité est initialisée à 0 !

                //Bonus:
                P->il_a_n_ouvrage=0;

                *TE=P;
                Q=P;
                for (i=2; i<=N_Etud; i++)
                {
                        P=malloc(sizeof(Etudiant));
                        printf ("\n\t Etudiant %d :",i);

                        printf ("\n\t\t Matricule= ");
                        scanf ("%s", &P->Matricule);
                        fflush (stdin);

                        printf ("\n\t\t Abonnement ('C': Classique ou 'P': Premium <-Majuscule->)=  ");
                        do
                                scanf("%c",&P->Abonnement);
                        while (P->Abonnement!='C' && P->Abonnement!='P');
                        fflush (stdin);

                        printf ("\n\t\t p%cnalit%c = 0 \n(par defaut, vous pouvez le changer dans la liste des actions).",130,130);
                        P->Penalite=0; //selon l'ennoncé la pénalité est initialisée à 0 !

                        //Bonus:
                        P->il_a_n_ouvrage=0;

                        Q->suiv=P;
                        Q=P;
                }

                Q->suiv=NULL;
        }
}

void AjouterEtud (EListe *TE, int i)
{
        EListe Q,P;

        //INSERTION A LA FIN :
        P=malloc(sizeof(Etudiant));
        printf ("\n\t Nouveau Etudiant %d :",i);

        printf ("\n\t\t Matricule= ");
        scanf ("%s", &P->Matricule);
        fflush (stdin);

        printf ("\n\t\t Abonnement ('C': Classique ou 'P': Premium <-Majuscule->)=  ");
        do
        {
                scanf ("%c", &P->Abonnement);
        }
        while (P->Abonnement!='C' && P->Abonnement!='P');


        printf ("\n\t\t p%cnalit%c = 0 \n(par defaut, vous pouvez la modifier dans la liste des actions).\n",130,130);
        P->Penalite=0; //selon l'ennoncé la pénalité est initialisée à 0 !

        //Bonus:
        P->il_a_n_ouvrage=0;

        P->suiv=NULL;
        Q=*TE;

        while (Q->suiv!=NULL)
        {
                Q=Q->suiv;
        }
        Q->suiv=P;
}

void AfficherListeEtudiants (EListe TE)
{
        int i=1;
        if (TE==NULL)
        {
                printf("\n\n Erreur: La liste des %ctudiants est VIDE !\n",130);
                //Beep(1000,800); //cette procedure AfficherListeEtudiant n'est pas demandé dans l'ennoncé
                Sleep(3*tps);
        }
        else
        {
                while (TE!=NULL)
                {
                        printf ("\n\n\t Etudiant %d :",i);

                        printf ("\n\t\t - Matricule: %s",TE->Matricule);
                        if (TE->Abonnement=='C')
                                printf ("\n\n\t\t - Type d'abonnement: Classique");
                        else
                                printf ("\n\n\t\t - Type d'abonnement: Premium");

                        printf ("\n\n\t\t - Total des p%cnalit%cs: %d",130,130,TE->Penalite);

                        TE=TE->suiv;
                        i++;

                        Sleep(5*tps);
                }
        }
}

int ExisteMatric(EListe TE, char Matricule [13])
{
        if (TE!=NULL)
        {
                while(TE!=NULL)
                {
                        if (strcmp(TE->Matricule,Matricule)==0)
                                return 1;

                        TE=TE->suiv;
                }
        }
        Beep(1000,500);
        printf("\n ERREUR: Ce Matricule n'existe pas !!\n");
        return (0);
}

void ConsulterMembre(EListe TE, char Matricule [13])
{
        if (ExisteMatric(TE,Matricule)!=0)
        {
                //chercher l'etudiant
                while (TE!=NULL && strcmp(TE->Matricule,Matricule)!=0) //boucler tnq c po fini et l'element n'et pas encore trouvé
                {
                        TE=TE->suiv;
                }
                //affichage:
                printf ("\n\n\t Etudiant :");
                printf ("\n\t\t - Matricule: %s",TE->Matricule);
                Sleep(3*tps);
                if (TE->Abonnement=='C')
                        printf ("\n\n\t\t - Type d'abonnement: Classique");
                else
                        printf ("\n\n\t\t - Type d'abonnement: Premium");
                Sleep(3*tps);
                printf ("\n\n\t\t - Total des p%cnalit%cs: %d",130,130,TE->Penalite);
        }
}

void SupprimerMembre (EListe *TE, char Matricule [13], int *Creer_E)
{
        EListe P,Q;
        int sup =0;

        P=*TE;
        //if (ExisteMatric(P,Matricule))
        //{
                //si c'est la tete => suppression au debut:
                P=*TE;
                //si l'element a supprimer est la tête :
                if(strcmp(P->Matricule,Matricule)==0)
                {
                        *TE=P->suiv;
                        free(P);
                        sup=1;
                        printf("\n L'%ctudiant (Matricule= %s) a %ct%c supprim%c avec succ%cs\n",130,Matricule,130,130,130,130);
                }
                Q=*TE;
                P=Q->suiv;
                //suppression au milieu :
                while(P->suiv!=NULL && sup==0)
                {
                        if (strcmp(P->Matricule,Matricule)==0)
                        {
                                Q->suiv=P->suiv;
                                free(P);
                                sup=1;
                                printf("\n L'%ctudiant (Matricule= %s) a %ct%c supprim%c avec succ%cs\n",130,Matricule,130,130,130,130);
                                sup=1;
                        }
                        P=P->suiv;
                        Q=Q->suiv;
                }
                //suppression a la fin:
                if (P->suiv==NULL && strcmp(P->Matricule,Matricule)==0)
                {
                        Q->suiv=NULL;
                        free (P);
                        printf("\n L'%ctudiant (Matricule= %s) a %ct%c supprim%c avec succ%cs\n",130,Matricule,130,130,130,130);
                        sup=1;

                }
        //}


//soit la liste est vide soit l'etudiant n'existe pas (Cas d'erreur) :
        if (sup==0)
        {
                Beep(1000,500);
                printf("\n Cet Etudiant (Matricule= %s) n'existe pas !!\n",Matricule);
                Sleep(3*tps);
        }

        //pour ne pas avoir des problèmes lors de l'ajout d'un ouvrage après !
        if (*TE==NULL)
                Creer_E=0;
}

void ModifierMembre (EListe *TE, char Matricule [13])
{
        EListe P;
        int choix;

        P=*TE;

        if (ExisteMatric(P,Matricule)!=0)
        {
                //chercher l'etudiant
                P=*TE;
                while (P!=NULL && strcmp(P->Matricule,Matricule)!=0) //boucler tnq c po fini et l'element n'et pas encore trouvé
                {
                        P=P->suiv;
                }

                //Choisir le champs à modifier:
                printf ("\n Choisissez le num%cro du champs %c modifier:\n",130,133);
                printf("\n\t 1>> Modifier le Matricule.");
                printf("\n\t 2>> Modifier le type d'abonnement.");
                printf("\n\t 3>> Modifier les p%cnalit%s.\n\t\t Modifier: ",130,130);
                scanf("%d",&choix);
                while (choix < 1 || choix>3)
                {
                        Beep (1000,500);
                        printf("\n\t CHOIX INCORRECTE !! Faites entrer un CHOIX VALIDE svp !!\n\t\t Choix: ");
                        scanf ("%d",&choix);
                }

                //MODIFIER LE champs choisi :
                switch (choix)
                {
                case 1:
                        //modifier le Matricule :
                        printf("\nEntrer le nouveau matricule.\n\t\t Nouveau Matricule=");
                        scanf("%s",&P->Matricule);
                        fflush(stdin);
                        break;
                case 2:
                        //Modifier le type d'abonnement :
                        printf("\nEntrer le nouveau type d'abonnement.\n\t\t Nouveau Type d'abonnement= ");
                        do
                                scanf("%c",&P->Abonnement);
                        while (P->Abonnement!='C' && P->Abonnement!='P');
                        if (P->Abonnement=='P')
                        {
                                P->Penalite=0; //dans le cas du type abonnement premium, la pénalité est tjrs 0;
                        }
                        break;
                default: //ie: choix=3
                        //Modifier la Pénalité :
                        if (P->Abonnement=='C')
                        {
                                printf("\nEntrer la nouvelle valeur de la p%cnalit%c.\n\t\t Nouvelle P%cnalit%c= ",130,130,130,130);
                                scanf("%d",&P->Penalite);
                                if (P->Penalite==Max_Penalite)
                                {
                                        //introduire cet etudiant dans la liste des Penalités
                                }
                        }
                        else
                                printf("\n Vous ne pouvez pas changer la p%cnalit%c d'un %ctudiant avec un type d'abonnement PREMIUM !\n",130,130,130);
                        break;
                }
        }
}




//_____________________________________________________________________________________________________________________________________________________________________

//....................................... primitives des Files .................................................................
//_____________________________________________________________________________________________________________________________________________________________________

//procedure Initfile
void Initfile(File *F)
{
        (*F).Tete=NULL;
        (*F).Queue=NULL;
}

//procedure Enfiler
void Enfiler(File *F, Telm x)
{
        EFile V;
        V=malloc(sizeof(CelluleF));
        V->Val=x;
        V->Suiv=NULL;
        if ((*F).Tete==NULL)
                (*F).Tete=V; //la file est vide
        else
                (*F).Queue->Suiv=V;
        (*F).Queue=V;
}

//procedure Defiler
void Defiler(File *F, Telm *x)
{
        EFile V;
        V=(*F).Tete;
        *x=V->Val; //ou  *x=(*F).Tete->Val;
        if ((*F).Tete==(*F).Queue)
        {
                (*F).Tete=NULL;
                (*F).Queue=NULL;
        }
        else
                (*F).Tete=(*F).Tete->Suiv; // ou  (*F).Tete=V->Suiv;
        free(V);
}

//Fonction Filevide
int Filevide(File F)
{
        if (F.Tete==NULL)
                return 1;
        else
                return 0;
}

//Fonction Tetefile
Telm Tetefile(File F)
{
        return F.Tete->Val;
}


//_________________________________________________________________________________________________________________________________________________

//**** Emprunts ***************************************************************************************************************************************

/* L'ajout d'un emprunt n'est pas comme l'ajout d'un ouvrage ou d'un etudiant
on ajoute les emmprunts un par un contrairement aux ouvrages et aux etudiants qui, on les ajoutait selon la demande de l'utilisateur
De plus, l'ajout des emprunts doit passer par plusieurs tests et controles(qu'on presente ci dessous)*/

int MatriculeNonInterdit (EListe TE, char Matriculee [13])
{
        EListe E,P;
        P=TE;
        E=TE;
        //1. matricule existe
        //2. penalité.etud<max=7
        //3. N.ouvrages.empruntés < Max.ouvrages.empruntés
        //printf("\nJe suis NONiNTERDIT");
        if (ExisteMatric(P, Matriculee))
        {
                //printf("\nJ'existe");
                while(strcmp(E->Matricule, Matriculee)!=0)
                {
                        //printf("c moi, P->matricule= %s et Matriculee= %s",TE->Matricule, Matriculee);
                        E=P->suiv;
                }
                //test:
                //printf("c moi, TE->Penalite= %d et MaxPenalite= %d",TE->Penalite,Max_Penalite);
                //printf("c moi, TE->il_a_n_ouvrage= %d et Max_Ouvrage_Eprunte= %d",TE->il_a_n_ouvrage,Max_Ouvrage_Emprunte);
                if (E->Penalite < Max_Penalite && E->il_a_n_ouvrage<Max_Ouvrage_Emprunte)
                {
                        //printf ("\n\nFELICITATIONs!!\n\n");
                        return (1);
                }
                else
                {
                        return 0;
                }
        }
        else
        {
                //Beep(1000,500);
                printf("\nAlors cet etudiant (Matricutle = %s) est interdit de l'emprunt !!\n",Matriculee);
                return (0);
        }
}

void DureeEmprunt (int *Duree_Emprunt)
{
        printf("\n\nVeuillez pr%cciser la dur%ce de l'emprunt.",130,130,133);
        printf("\n\t\t Dur%ce d'un emprunt (par jours)= ",130);
        scanf("%d",&*Duree_Emprunt);
        while (*Duree_Emprunt<=0)
        {
                Beep(1000,500);
                printf("\nVeuillez introduire un nombre de jours valide!\n\t\t Dur%ce d'un emprunt (par jours)= ",130);
                scanf("%d",&*Duree_Emprunt);
        }
}

void MaxEmprunt (int *Max_Ouvrage_Emprunte)
{
        printf("\n\nVeuillez pr%cciser le maximum d'ouvrages %c emprunter.",130,133);
        printf("\n\t\t Max= ");
        scanf("%d",&*Max_Ouvrage_Emprunte);
        while (*Max_Ouvrage_Emprunte<=0)
        {
                Beep(1000,500);
                printf("\nVeuillez introduire un nombre Max d'ouvrages valide!\n\t\t Max = ");
                scanf("%d",&*Max_Ouvrage_Emprunte);
        }
}

void DateFin(int Duree_Emprunt,int *f_day,int *f_mois,int *f_an)
{
        for(int i=1; i<=Duree_Emprunt; i++)
        {
                if((*f_mois)==1 || (*f_mois) ==3 || (*f_mois)==5 || (*f_mois)==7 || (*f_mois)==8 || (*f_mois)==10 || (*f_mois)==12)
                {
                        if((*f_day)<31)
                        {
                                (*f_day)++;
                        }
                        else    //day=31
                        {
                                (*f_day)=1;
                                if ((*f_mois)!=12)
                                {
                                        (*f_mois)++;
                                }
                                else
                                {
                                        (*f_mois)=1;
                                        (*f_an)++;
                                }
                        }
                }
                else
                {
                        if ((*f_mois)==2)
                        {
                                if ((*f_day)<28)
                                {
                                        (*f_day)++;
                                }
                                else
                                {
                                        //
                                        if ((*f_day)==28)
                                        {
                                                if ((*f_an)%4)
                                                {
                                                        (*f_day)++;
                                                }
                                                else
                                                {
                                                        (*f_day)=1;
                                                        (*f_mois)++;
                                                }
                                        }
                                        else //day ==29
                                        {
                                                (*f_day)=1;
                                                (*f_mois)++;
                                        }
                                        //
                                }
                        }
                        else
                        {
                                if((*f_day)<30)
                                {
                                        (*f_day)++;
                                }
                                else
                                {
                                        (*f_day)=1;
                                        (*f_mois)++;
                                }
                        }
                }
        }
        //printf("Date_fin : %d/ %d/ %d",*f_day,*f_mois,*f_an);
}

void CreerTeteEmprunt (PListe *TP, OListe TO, EListe TE, int Duree_Emprunt, int Max_Ouvrage_Emprunte)
{
        /*//les ouvrages_empruntés<Max_Ouvrage_Emprunte
        //au moins un exemplaire de livre est disponible (int existeExemplaire(OListe TO))
        //etudiant existe (cet etudiant n'existe pas dans la liste\n\t Ajouter un Nouveau Etudiant? (tapez 1 pour oui/ 0 pour non)\n\t\t Reponse: ")
        //cote correcte(existe)*/

        PListe P;
        OListe O;
        EListe E;

        char Matriculee [13];
        char cotee [15];
        int f_day, f_mois, f_an;

        printf ("\n\n\t NOUVEL Emprunt:");

        //*TP=NULL;
        O=TO;
        E=TE;

        //..................Les tests infinis.........................

        //tester le matricule:
        printf ("\n\t\t Matricule= ");
        scanf ("%s", &Matriculee);
        fflush (stdin);
        printf("je suis la");

        //printf("non interdit=%d",MatriculeNonInterdit(TE,Matriculee));
        if (MatriculeNonInterdit(TE,Matriculee)==0)
        {
                printf("hi1");
                //si le matricule n'existe pas ou etudiant est interdit de l'emprunt:
                printf("\n ERREUR: Matricule=%s incorrecte ou bien l'etudiant est interdit de l'emprunt.\n",Matriculee);
                Beep(1000,500);
        }
        else
        {
                printf("je suis la");
                //strcpy(P->Matricule,Matriculee);
                //tester la cote:
                printf ("\n\t\t Cote= ");
                scanf ("%s", &cotee);
                fflush(stdin);

                if (ExisteCote(O,cotee)==0) //si l'ouvrage n'exite pas :
                {
                        printf("\n ERREUR: Cote=%s est incorrecte ou n'existe pas!!\n",cotee);
                        Beep(1000,500);
                }
                else // si l'ouvrage existe :
                {
                        O=TO;
                        //printf("la cote existe");
                        if (ExemplaireDispo(O, cotee)==1) // s'il existe des exemplaires, on continue :
                        {
                                P=malloc(sizeof(Emprunt));

                                strcpy(P->Matricule,Matriculee);

                                strcpy(P->Cote,cotee);

                                //date debut :
                                P->Date_Debut.jour=day;
                                P->Date_Debut.mois=mois;
                                P->Date_Debut.an=an;
                                printf("\n  >>Date de l'emprunt est la date d'aujourd'hui: %d/%d/%d\n",P->Date_Debut.jour,P->Date_Debut.mois,P->Date_Debut.an);

                                //date du retour de l'ouvrage :
                                f_day=day;
                                f_mois=mois;
                                f_an=an;
                                //date fin (date debut + 1jour*Duree_Emprunt)
                                DateFin(Duree_Emprunt,&f_day,&f_mois,&f_an);
                                //affecter la date dans le champs Date :
                                P->Date_Fin.jour=f_day;
                                P->Date_Fin.mois=f_mois;
                                P->Date_Fin.an=f_an;

                                P->suiv=NULL;

                                *TP=P;

                                O=TO;
                                //modifier l'ouvrage emprunté:
                                while (O!=NULL && strcmp(O->Cote,cotee)!=0)
                                {
                                        O=O->suiv;
                                }
                                O->n_emprunt=O->n_emprunt+1; //bonus: incrementer le Nombre de fois ce livre a été emprunté
                                O->n_exmplr_biblio=O->n_exmplr_biblio-1; //decrementer les exmplr dispo
                                O->n_exmplr_emprnt=O->n_exmplr_emprnt+1; //incrementer les exemplaires non rendus
                                //printf("O->emprunt= %d ,  O->n_exmplr_biblio=%d \n\n",O->n_emprunt, O->n_exmplr_biblio);

                                //bonus :
                                E=TE;
                                while (strcmp(E->Matricule,Matriculee)!=0)
                                {
                                        E=E->suiv;
                                }
                                E->il_a_n_ouvrage=E->il_a_n_ouvrage+1;
                                //printf("TP->Cote: %s ; P->Cote: %s",(*TP)->Cote, P->Cote);
                                //printf("TP->Cote: %s ; P->Cote: %s",(*TP)->Matricule , P->Matricule);
                        }
                        else //s'il n'y a pas assez d'exemplaires, on ajoute l'etudiant a la file d'attente :
                        {
                                printf("\n >>>Y a pas assez d'exemplaires! \n");
                                //ajouter dans la file:

                                //initialiser les deux files (FileP et FileC) en haut !

                                /*if (P->Matricule=='P')
                                {
                                        //ajouter dans File premium:
                                }
                                else
                                {
                                        //ajouter dans File classique:
                                }*/
                                printf("\n\n >>> L'etudiant est ajouté dans la file d'attente !");
                        }
                }
        }
}

void AjouterEmprunt (PListe *TP, OListe TO, EListe TE)
{
        PListe P,Q;
        OListe O;
        EListe E;

        char Matriculee [13];
        char cotee [15];
        int f_day, f_mois, f_an;

        printf ("\n\n\t NOUVEL Emprunt:");

        O=TO;
        E=TE;

        //..................Les tests infinis.........................

        //tester le matricule:
        printf ("\n\t\t Matricule= ");
        scanf ("%s", &Matriculee);
        fflush (stdin);

        //printf("non interdit=%d",MatriculeNonInterdit(TE,Matriculee));
        if (MatriculeNonInterdit(E,Matriculee)==0)
        {
                //si le matricule n'existe pas ou etudiant est interdit de l'emprunt:
                printf("\n ERREUR: Matricule=%s incorrecte ou bien l'etudiant est interdit de l'emprunt.\n",Matriculee);
                Beep(1000,500);
        }
        else
        {
                //strcpy(P->Matricule,Matriculee);
                //tester la cote:
                printf ("\n\t\t Cote= ");
                scanf ("%s", &cotee);
                fflush(stdin);

                if (ExisteCote(O,cotee)==0) //si l'ouvrage n'exite pas :
                {
                        printf("\n ERREUR: Cote=%s est incorrecte ou n'existe pas!!\n",cotee);
                        Beep(1000,500);
                }
                else // si l'ouvrage existe :
                {
                        O=TO;
                        //printf("la cote existe");
                        if (ExemplaireDispo(O, cotee)==1) // s'il existe des exemplaires, on continue :
                        {
                                P=malloc(sizeof(Emprunt));

                                strcpy(P->Matricule,Matriculee);

                                strcpy(P->Cote,cotee);

                                //date debut :
                                P->Date_Debut.jour=day;
                                P->Date_Debut.mois=mois;
                                P->Date_Debut.an=an;
                                printf("\n  >>Date de l'emprunt est la date d'aujourd'hui: %d/%d/%d\n",P->Date_Debut.jour,P->Date_Debut.mois,P->Date_Debut.an);

                                //date du retour de l'ouvrage :
                                f_day=day;
                                f_mois=mois;
                                f_an=an;
                                //date fin (date debut + 1jour*Duree_Emprunt)
                                DateFin(Duree_Emprunt,&f_day,&f_mois,&f_an);
                                //affecter la date dans le champs Date :
                                P->Date_Fin.jour=f_day;
                                P->Date_Fin.mois=f_mois;
                                P->Date_Fin.an=f_an;

                                P->suiv=NULL;

                                Q=*TP;

                                while (Q->suiv!=NULL)
                                {
                                        Q=Q->suiv;
                                }
                                Q->suiv=P;

                                //modifier l'ouvrage emprunté:
                                O=TO;
                                while (O!=NULL && strcmp(O->Cote,cotee)!=0)
                                {
                                        O=O->suiv;
                                }
                                O->n_emprunt=O->n_emprunt+1; //bonus: incrementer le Nombre de fois ce livre a été emprunté
                                O->n_exmplr_biblio=O->n_exmplr_biblio-1; //decrementer les exmplr dispo
                                O->n_exmplr_emprnt=O->n_exmplr_emprnt+1; //incrementer les exemplaires non rendus
                                //printf("O->emprunt= %d ,  O->n_exmplr_biblio=%d \n\n",O->n_emprunt, O->n_exmplr_biblio);

                                //bonus :
                                E=TE;
                                while (strcmp(E->Matricule,Matriculee)!=0)
                                {
                                        E=E->suiv;
                                }
                                E->il_a_n_ouvrage=E->il_a_n_ouvrage+1;
                                //printf("TP->Cote: %s ; P->Cote: %s",(*TP)->Cote, P->Cote);
                                //printf("TP->Cote: %s ; P->Cote: %s",(*TP)->Matricule , P->Matricule);
                        }
                        else //s'il n'y a pas assez d'exemplaires, on ajoute l'etudiant a la file d'attente :
                        {
                                printf("\n >>>Y a pas assez d'exemplaires! \n");
                                //ajouter dans la file:

                                //initialiser les deux files (FileP et FileC) en haut !

                                /*if (P->Matricule=='P')
                                {
                                        //ajouter dans File premium:
                                }
                                else
                                {
                                        //ajouter dans File classique:
                                }*/
                                printf("\n\n >>> L'etudiant est ajouté dans la file d'attente !");
                        }
                }
        }
}

void AfficherListeEmprunts(PListe TP)
{
        PListe P;
        P=TP;
        int i=1;
        while (P!=NULL)
        {
                printf ("emprunt %d :",i);
                printf ("\n\n\t Matricule= %s",P->Matricule);
                printf ("\n\t Cote= %s",P->Cote);
                printf ("\n\t Date du debut de l'emprunt= %02d/%02d/%d",P->Date_Debut.jour,P->Date_Debut.mois,P->Date_Debut.an);
                printf ("\n\t Date de la fin de l'emprunt= %02d/%02d/%d",P->Date_Fin.jour,P->Date_Fin.mois,P->Date_Fin.an);

                P=P->suiv;
                i++;
        }
}

void ConsulterEmprunt (PListe TP, char cote_Ouvrage[15], char Matricule [13])
{
        int erreur=1;
        PListe P;
        P=TP;

        while (P!=NULL) //on a pu faire comme la consultation d'un membre mais dans ce cas la, l'etudiant peut emprunter le meme livre plusieurs fois, du coup il faut les aficher tous!!
        {
                if (strcmp(P->Cote ,cote_Ouvrage)==0 && strcmp(P->Matricule,Matricule)==0)
                {
                        printf ("\n\n\t Matricule= %s",P->Matricule);
                        printf ("\n\t Cote= %s",P->Cote);
                        printf ("\n\t Date du debut de l'emprunt= %02d/%02d/%d",P->Date_Debut.jour,P->Date_Debut.mois,P->Date_Debut.an);
                        printf ("\n\t Date de la fin de l'emprunt= %02d/%02d/%d",P->Date_Fin.jour,P->Date_Fin.mois,P->Date_Fin.an);
                        erreur=0;
                }
                P=P->suiv;
        }
        if (erreur==1)
        {
                printf("\n\n >>> Cet emprunt n'existe pas !");
                Beep(1000,500);
        }
}

void SupprimerEmprunt (PListe *TP, char cote_Ouvrage[15], char Matricule [13])
{
        //j'ai choisi de supprimer toutes les occurences de la combinaison (cote-matricule)
        PListe P,Q;
        int sup =0;

        P=*TP;
        //si l'element a supprimer est la tête :
        if (strcmp(P->Cote ,cote_Ouvrage)==0 && strcmp(P->Matricule,Matricule)==0)
        {
                *TP=P->suiv;
                free(P);
                printf("\n L'%ctudiant (Matricule= %s) a %ct%c supprim%c avec succ%cs\n",130,Matricule,130,130,130,130);
                sup=1;
        }
        Q=*TP;
        P=Q->suiv;

        //suppression au milieu :
        while(sup==0 && P->suiv!=NULL)
        {
                if (strcmp(P->Cote ,cote_Ouvrage)==0 && strcmp(P->Matricule,Matricule)==0)
                {
                        Q->suiv=P->suiv;
                        free(P);
                        printf("\n L'%ctudiant (Matricule= %s) a %ct%c supprim%c avec succ%cs\n",130,Matricule,130,130,130,130);
                        sup=1;
                }
                P=P->suiv;
                Q=Q->suiv;
        }

        //suppression a la fin:
        if (P->suiv==NULL && sup==0 )
        {
                if (strcmp(P->Cote ,cote_Ouvrage)==0 && strcmp(P->Matricule,Matricule)==0)
                {
                        Q->suiv=NULL;
                        free (P);
                        printf("\n L'%ctudiant (Matricule= %s) a %ct%c supprim%c avec succ%cs\n",130,Matricule,130,130,130,130);
                        sup=1;
                }
        }

        // si la combinaison (cote_matricule) n'existe pas :
        if (sup==0)
        {
                Beep(1000,800);
                printf("\n Cet Emprunt n'existe pas !!\n");
                Sleep(3*tps);
        }

}

void ModifierEmprunt (PListe *TP, char cote_Ouvrage[15], char Matricule [13])
{
        int erreur=1;
        int champs;
        char matriculee [13];
        char cotee [15];
        PListe P;
        P=TP;

        while (P!=NULL && erreur==1) //on a pu faire comme la consultation d'un membre mais dans ce cas la, l'etudiant peut emprunter le meme livre plusieurs fois, du coup il faut les aficher tous!!
        {
                if (strcmp(P->Cote ,cote_Ouvrage)==0 && strcmp(P->Matricule,Matricule)==0)
                {
                        printf ("\n\n\t 1>> Matricule.");
                        printf ("\n\t 2>> Cote=");
                        printf ("\n\t 3>> Date du debut de l'emprunt.");
                        printf ("\n\t 4>> Date de la fin de l'emprunt.");

                        printf ("\n\t\t\t choisissez un champs.\n\t\t\t\t Champs = ");
                        scanf("%d",&champs);

                        while (champs<1 || champs>4);
                        {
                                printf("ERREUR: CHAMPS NON VALIDE!\n\t\t\t\t Champs = ");
                                Beep(1000,500);
                                scanf("%d",&champs);
                        }
                        erreur=0;
                }
                P=P->suiv;
        }
        switch (champs)
        {
                case 1 :
                        //modifier le matricule:
                        printf ("\n   Nouveau Matricule =");
                        scanf ("%s",&matriculee);
                        fflush(stdin);

                        if (ExisteMatric(TE, matriculee)!=0)
                        {
                                strcpy(P->Matricule,matriculee);
                                printf ("\n\t Matricule modifié avec succées!");
                        }
                        else
                        {
                                printf ("\n Matricule incorrecte !");
                        break;
                case 2 :
                        //modifier la cote:
                        printf ("\n   Nouvelle Cote =");
                        scanf("%s",&cotee);
                        fflush(stdin);

                        if (ExisteCote(TO, cotee)!=0)
                        {
                                strcpy(P->Cote,cotee);
                                printf ("\n\tcote modifiée avec succées!");
                        }
                        else
                        {
                                printf ("\n Cote incorrecte !");
                        }
                        break;
                case 3:
                        //modifier la date du debut:
                        printf ("\n Vous ne pouvez pas changer ce champs pour des raisons de sécurité !");
                        break;
                case 4:
                        //modifier la date de la fin:
                        printf ("\n Vous ne pouvez pas changer ce champs pour des raisons de sécurité !");
                        break;
                }

                if (erreur==1)
                {
                        printf("\n\n >>> Cet emprunt n'existe pas !");
                        Beep(1000,500);
                }
        }
}



int main()
{
//_____________________________________________________________________________________________________________________________________________
        //bienvenue
        printf ("\n\t***Bienvenue dans l'application console de Gestion de Biblioth%cque***\n\n",138);

        //la date d'aujourd'hui:
        time(&now);
        struct tm *local = localtime(&now);
        day = local->tm_mday;
        mois = local->tm_mon + 1;
        an = local->tm_year + 1900;
        printf("Aujourd'hui est : %02d/%02d/%d\n", day, mois, an);
//_____________________________________________________________________________________________________________________________________________
        //choisir
        printf ("\n\n\tChoisissez une Action :\n");
        AfficherActions();
//_____________________________________________________________________________________________________________________________________________
        do
        {
                //choisir une action...
                printf ("\n\n\t\t\t-----> Action: ");
                scanf ("%d", &choix);
                while (choix<0 || choix>20)
                {
                        printf ("\n ERREUR: Action Invalide !!\n\t\t\t-----> Action: ");
                        Beep (1000,600);
                        scanf ("%d", &choix);
                }

                //      executer une action:
                switch (choix)
                {
                case 1:
                        //      ajouter des ouvrages:
                        printf ("  - Entrer le nombre d'ouvrages %c introduire\n\t\t\t >>>N= ",133);
                        scanf("%d", &N_Ouvrage);
                        while (N_Ouvrage<=0)
                        {
                                Beep(1500,500);
                                printf("\nEntrer un nombre Valide SVP !");
                                scanf("%d", &N_Ouvrage);
                        }
                        //      creer la liste ou ajouter un element ?
                        if (creer_O==0 || TO==NULL)
                        {
                                CreerListeOuvrage (&TO, N_Ouvrage);
                                creer_O=1;
                        }
                        else
                        {
                                for (int i=1; i<=N_Ouvrage; i++)
                                        AjouterOuvrage(&TO, i);
                        }
                        break;
                case 2:
                        //      Consulter la liste des ouvrages
                        AfficherListeOuvrages(TO);
                        break;
                case 3:
                        //      Supprimer un ouvrage :
                        printf("\n\nPour supprimer un ouvrage veuillez introduire sa cote\n\t\tCote=");
                        scanf("%s", &cote_Ouvrage);
                        fflush(stdin);

                        SupprimerOuvrage(&TO, cote_Ouvrage, &creer_O);
                        break;
                case 4:
                        //      Verifier la disponibilité des exemplaires:
                        printf("V%crifier la disponibilit%c de l'ouvrage : \n\t\t Cote= ",130,130);
                        scanf("%s",&cote_Ouvrage);

                        if (ExemplaireDispo(TO, cote_Ouvrage))
                                printf("\n\t\t\t Cet ouvrage (cote= %s) est DISPONIBLE !\n",cote_Ouvrage);
                        else
                                printf("\n\t\t\t Cet ouvrage (cote= %s) est NON DISPONIBLE !!\n",cote_Ouvrage);
                        break;
                case 5:
                        //      ajouter des Etudiants:
                        printf ("  - Entrer le nombre d'etudiants %c ajouter dans la liste\n\t\t\t >>>N= ",133);
                        scanf("%d", &N_Etud);
                        while (N_Etud<=0)
                        {
                                Beep(2500,500);
                                printf("\nEntrer un nombre valide SVP !!");
                                scanf("%d", &N_Etud);
                        }
                        //      creer une liste ou ajout ?
                        if (Creer_E==0 || TE==NULL)
                        {
                                CreerListeEtud (&TE, N_Etud);
                                Creer_E=1;
                        }
                        else
                        {
                                for (int i=1; i<=N_Etud; i++)
                                        AjouterEtud(&TE, i);
                        }
                        break;
                case 6:
                        //      CONSULTER UN ETUDIANT :
                        printf ("\n  Entrer le Matricule de l'%ctudiant: \n\t\t MAtricule= ",130);
                        scanf("%s",&Matricule);
                        fflush(stdin);
                        if (ExisteMatric(TE,Matricule))
                        {
                                ConsulterMembre(TE,Matricule);
                        }
                        break;
                case 7:
                        //      SUPPRIMER UN MEMBRE :
                        printf("\n Entrer le matricule de l'%ctudiant.\n\t\t Matricule= ",130);
                        scanf("%s",&Matricule);
                        fflush(stdin);
                        if (ExisteMatric(TE, Matricule))
                        {
                                SupprimerMembre(&TE, Matricule, &Creer_E);
                        }
                        break;
                case 8:
                        //      Modifier un Membre :
                        printf("\n Entrer le matricule de l'%ctudiant %c modifier.\n\t\t Matricule= ",130,133);
                        scanf("%s",&Matricule);
                        fflush(stdin);
                        if (ExisteMatric(TE, Matricule))
                        {
                                ModifierMembre (&TE, Matricule);
                        }
                        break;
                case 9:
                        //       AJOUTER UN EMPRUNT :
                        //puisqu'on sait pas la durée ni la max des ouvrages on demande à l'utilisateur de les introduire :
                        if(Duree_Emprunt==0)
                        {
                                DureeEmprunt(&Duree_Emprunt);
                                //printf("%d",Duree_Emprunt);
                        }
                        if(Max_Ouvrage_Emprunte==0)
                        {
                                MaxEmprunt(&Max_Ouvrage_Emprunte);
                                //printf("%d",Max_Ouvrage_Emprunte);
                        }
                        //ajouter emprunt:
                        if (creer_P==0)
                        {
                                CreerTeteEmprunt(&TP, TO, TE, Duree_Emprunt, Max_Ouvrage_Emprunte);
                                creer_P=1;
                                //printf("\n\ndebut: jour= %02d mois= %02d an=%d",TP->Date_Debut.jour, TP->Date_Debut.mois, TP->Date_Debut.an);
                                //printf("\n\nfin: jour= %02d mois= %02d an=%d",TP->Date_Fin.jour, TP->Date_Fin.mois, TP->Date_Fin.an);
                        }
                        else
                        {
                                AjouterEmprunt(&TP, TO, TE);
                        }
                        //printf("cote: %s, \nmatricule: %s",TP->Cote,TP->Matricule);
                        break;
                case 10:
                //      CONSULTER UN Emprunt :
                        printf ("\n  Entrer le Matricule de l'%ctudiant: \n\t\t MAtricule= ",130);
                        scanf("%s",&Matricule);
                        fflush(stdin);
                        if (ExisteMatric(TE,Matricule)) // tester l'existence de l'etudiant
                        {
                                printf ("\n  Entrer la cote de l'ouvrage: \n\t\t Cote= ");
                                scanf("%s",&cote_Ouvrage);
                                fflush(stdin);
                                if(ExisteCote(TO,cote_Ouvrage)) // tester l'existence de l'ouvrage
                                {
                                        ConsulterEmprunt(TP, cote_Ouvrage, Matricule);
                                }
                                else
                                {
                                        printf("Cet ouvrage n'existe pas !!!");
                                        Beep (1000,500);
                                }
                        }
                        break;
                case 11:
                //      SUPPRIMER UN Emprunt :
                        printf ("\n  Entrer le Matricule de l'%ctudiant: \n\t\t MAtricule= ",130);
                        scanf("%s",&Matricule);
                        fflush(stdin);
                        if (ExisteMatric(TE,Matricule)) // tester l'existence de l'etudiant
                        {
                                printf ("\n  Entrer la cote de l'ouvrage: \n\t\t Cote= ");
                                scanf("%s",&cote_Ouvrage);
                                fflush(stdin);
                                if(ExisteCote(TO,cote_Ouvrage)) // tester l'existence de l'ouvrage
                                {
                                        SupprimerEmprunt (&TP, cote_Ouvrage, Matricule);
                                }
                                else
                                {
                                        printf("Cet ouvrage n'existe pas !!!");
                                        Beep (1000,500);
                                }
                        }
                        break;
                case 12:
                        printf ("\n  Entrer le Matricule de l'%ctudiant: \n\t\t MAtricule= ",130);
                        scanf("%s",&Matricule);
                        fflush(stdin);
                        if (ExisteMatric(TE,Matricule)) // tester l'existence de l'etudiant
                        {
                                printf ("\n  Entrer la cote de l'ouvrage: \n\t\t Cote= ");
                                scanf("%s",&cote_Ouvrage);
                                fflush(stdin);
                                if(ExisteCote(TO,cote_Ouvrage)) // tester l'existence de l'ouvrage
                                {
                                        ModifierEmprunt(&TP, cote_Ouvrage, Matricule);
                                }
                                else
                                {
                                        printf("Cet ouvrage n'existe pas !!!");
                                        Beep (1000,500);
                                }
                        }
                        break;
                case 13:


                        break;
                case 14:


                        break;
                case 15:


                        break;
                case 16:


                        break;
                case 17:
                        //Modifier la durée des emprunts :
                        DureeEmprunt(&Duree_Emprunt);
                        printf ("\n la dur%ce d'un emprunt est d%csormais: %d jour(s). ",130,130,Duree_Emprunt);
                        //changer la durée d'emprint de CHAQUE Emprunt
                        //incrementer(matricule, Duree_Emprunt);
                        break;
                case 18:
                        //Modifier le Max des Emprunts :
                        MaxEmprunt(&Max_Ouvrage_Emprunte);
                        printf ("\n le nombre maximum d'ouvrages est d%csormais: %d ouvrage(s) emprunt%c(s) par %ctudiant.",130,Max_Ouvrage_Emprunte,130,130);
                        break;
                case 19:


                        break;
                case 20:


                        break;
                case 0:
                        printf("\n\n \t\t\t\t\t\t A tr%cs bientot !\n\n",138);
                        return 0;
                        break;
                }
                AfficherListeEmprunts(TP);
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                Sleep(3*tps);
                printf("\n\n\t 1>> Choisir une autre action.\n");
                printf("\n\t 2>> Afficher la liste des Actions.\n");
                printf("\n\t 0>> Quitter.\n");

                printf ("\n\n\t\t\t-----> Choix: ");
                scanf ("%d", &choix);
                while (choix<0 || choix>2)
                {
                        printf("\n ERREUR: Choix invalide !!\n\t\t\t-----> Choix: ");
                        Beep(3000,500);
                        scanf ("%d", &choix);
                }

                if (choix==2)
                        AfficherActions();
        }
        while (choix!=0);

        printf("\n\n \t\t\t\t\t A tr%cs bientot !\n\n",138);

        //EXTRA : tester si la liste des etudiants est bien remplie !
        AfficherListeEtudiants (TE);



        return 0;
}