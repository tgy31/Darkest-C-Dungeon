#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct{
    char nom[20];
    int att;
    int def;
    int HPmax;
    int rest;
}classe_personnage;

typedef struct{
    char nom[20];  
    classe_personnage classe;
    int HP;
    int str;
    int nbcomb;
}personnage;

typedef struct{
    char nom[20];
    int attbonus;
    int defbonus;
    int HPbonus;
    int restbonus;
    int strred;
}accessoire;

typedef struct{
    char nom[20];
    int niveau;
    int att_enn;
    int def_enn;
    int HP_enn;
    int attstr_enn;
}ennemi;

typedef struct{
    int prix;
    accessoire exemple;
}accessoire_roulotte;

typedef struct Cellule1{ //PERSONNAGE DISPONIBLE, PERSONNAGE COMBATTANT, PERSONNAGE SANITARIUM, PERSONNAGE TAVERNE
    personnage perso;
    struct Cellule1 *suivant;
}Cellule1, *ListePers;

typedef struct Cellule2{ //ACCESSOIRE DISPONIBLE, ACCESSOIRE EN VENTE
    accessoire access;
    struct Cellule2 *suivant;
}Cellule2, *ListeAccess;





void affiche_classe(classe_personnage exemple){
    printf("\nClasse %s: att: %d, def: %d, HPmax: %d, rest: %d\n", exemple.nom, exemple.att, exemple.def, exemple.HPmax, exemple.rest);
}

void affiche_personnage(personnage exemple){
    printf("\n%s\nClasse %s, att: %d, def: %d, HP/HPmax: %d/%d, rest: %d, str: %d, nbcomb: %d\n", exemple.nom, exemple.classe.nom, exemple.classe.att, exemple.classe.def, exemple.HP, exemple.classe.HPmax, exemple.classe.rest, exemple.str, exemple.nbcomb);
}

void affiche_accesoire(accessoire exemple){
    printf("\nAccesoire %s: attbonus: +%d, defbonus: +%d, HPbonus: +%d, restbonus: +%d, strred: %d\n", exemple.nom, exemple.attbonus, exemple.defbonus, exemple.HPbonus, exemple.restbonus, exemple.strred);
}

void affiche_ennemi(ennemi exemple){
    printf("\nEnnemi %s: niveau: %d, att: %d, def: %d, HP: %d, attstr: %d\n", exemple.nom, exemple.niveau, exemple.att_enn, exemple.def_enn, exemple.HP_enn, exemple.attstr_enn);
}

int saisie_choix() {
    int choix;
    int valid = 0;
    
    while (!valid) {

        if (scanf("%d", &choix) == 1) {

            if (choix >= 1 && choix <= 4) {
                valid = 1;  
            } 
            else {
                printf("Choix invalide. Le nombre doit être entre 1 et 4.\n");
            }
        } 
        else {
            printf("Saisie invalide. Veuillez entrer un entier.\n");
            
            while (getchar() != '\n');
        }
    }
    
    return choix;
}

ListePers alloueCellule1(personnage exemple){
    ListePers tmp;
    tmp = (Cellule1 *)malloc(sizeof(Cellule1));
    if(tmp == NULL){
        return NULL; //SI ECHEC
    }
    else{
        tmp->perso = exemple;
        tmp->suivant = NULL;
        return tmp;//SINON
    }
}

ListeAccess alloueCellule2(accessoire exemple){
    ListeAccess tmp;
    tmp = (Cellule2 *)malloc(sizeof(Cellule2));
    if(tmp == NULL){
        return NULL; //SI ECHEC
    }
    else{
        tmp->access = exemple;
        tmp->suivant = NULL;
        return tmp;//SINON
    }
}

int inserePersonnageTete(ListePers *lst, personnage exemple){
    ListePers tmp = alloueCellule1(exemple);
    if(tmp != NULL){
        tmp->suivant = *lst;
        *lst = tmp;
        return 1;//SINON
    }
    return 0;//SI ECHEC
}

int inserePersonnageSuite(ListePers *lst, personnage exemple){
    ListePers newCell = alloueCellule1(exemple);

    if(*lst != NULL && newCell != NULL){
        ListePers temp = *lst;
        while(temp->suivant != NULL){
            temp = temp->suivant;
        }
        temp->suivant = newCell;
        return 1;//SINON
    }
    return 0;//SINON

}

int supprimePersonnage(ListePers *lst, char nom[]){
    if (*lst == NULL){ //LISTE VIDE
        return 0;;
        
    }
    ListePers temp = *lst;
    ListePers prev = NULL;

    if(strcmp(temp->perso.nom, nom) == 0){//SI ELEM A TROUVER EST PREMIER ELEM
        *lst = temp->suivant;
        free(temp);
        return 1;
    }

    while(temp != NULL && (strcmp(temp->perso.nom, nom) != 0)){ //SINON
        prev = temp;
        temp = temp->suivant;
    }

    if(temp == NULL){ //PAS TROUVE
        return 0;
    }

    prev->suivant = temp->suivant;//SINON
    free(temp);
    return 1;
}


    
void sanitarium(personnage *exemple){
    srand(time(NULL));
    int chance = rand() %5;
    if((exemple->HP + 5 + chance) > exemple->classe.HPmax){
        
        int augmentation = exemple->classe.HPmax - exemple->HP;
        printf("\n%s est passé au sanitarium!      +%d HP!\n",exemple->nom ,augmentation);
        exemple->HP = exemple->classe.HPmax;
    }
    else{
        exemple->HP += 5 + chance;
        printf("\n%s est passé au sanitarium!      +%d HP!\n",exemple->nom , 5+chance);
    }
}

void taverne(personnage *exemple){
    if(exemple->str - 25 < 0){
        printf("\n%s est passé à la taverne!      -%d stress!\n", exemple->nom, exemple->str);
        exemple->str = 0;
    }
    else{
        exemple->str -= 25;
        printf("\n%s est passé à la taverne!      -25 stress!\n", exemple->nom);
    }
}

void roulotte(){

}

void afficheListePersoRec(ListePers lst){
    if(lst != NULL){
        printf("Nom: %s\n", lst->perso.nom);
        afficheListePersoRec(lst->suivant);
    }
    printf("\nfin affichage perso\n");
}

void afficheListePerso(ListePers lst) {
    while (lst != NULL) {  // Parcourt la liste tant qu'il y a des éléments
        printf("Nom: %s\n", lst->perso.nom);
        lst = lst->suivant;  // Avance au suivant
    }
    printf("\nfin affichage personage\n\n");
}

void libererMem1(ListePers lst){
    Cellule1* courant = lst;
    while (courant != NULL) {
        Cellule1* temp = courant;         
        courant = courant->suivant;      
        free(temp);                      
    }
}

void libererMem2(ListeAccess lst){
    Cellule2* courant = lst;
    while (courant != NULL) {
        Cellule2* temp = courant;         
        courant = courant->suivant;      
        free(temp);                      
    }
}
 
int main(){
    classe_personnage Furie = {"Furie", 13, 0, 20, 0};
    classe_personnage Vestale = {"Vestale", 3, 0, 20, 10};
    classe_personnage Chasseur_de_primes = {"Chasseur de primes", 7, 3, 25, 3};
    classe_personnage Maitre_chien = {"Maître chien", 10, 6, 17, 5};

    /*affiche_classe(Furie);
    affiche_classe(Vestale);
    affiche_classe(Chasseur_de_primes);
    affiche_classe(Maitre_chien);*/


    personnage Boudicca = {"Boudicca", Furie, 18, 23, 0};
    personnage Junia = {"Junia", Vestale, 20, 0, 0};
    personnage William = {"William", Maitre_chien, 17, 0, 0};
    personnage Tardif = {"Tardif", Chasseur_de_primes, 25, 0, 0};

    //affiche_personnage(Boudicca);
    //affiche_personnage(Junia); 

    ListePers liste_perso = NULL;
    printf("affichage 1\n");
    afficheListePerso(liste_perso);
    inserePersonnageTete(&liste_perso, Boudicca);
    inserePersonnageTete(&liste_perso, Junia);
    printf("affichage 2\n");
    afficheListePerso(liste_perso);
    inserePersonnageSuite(&liste_perso, William);
    afficheListePerso(liste_perso);

    //int temp = supprimePersonnage(&liste_perso, "Junia");
    //printf("%d\n\n", temp);
    //printf("affichage 3\n");
    //afficheListePerso(liste_perso);



    libererMem1(liste_perso);



    /*accessoire Pendentif_tranchant = {"Pendentif tranchant", 5, 1, 0, 0, 0};
    accessoire Calice_de_jeunesse = {"Calice de jeunesse", 0, 3, 5, 0, 5};

    affiche_accessoire(Pendentif_tranchant);
    affiche_accessoire(Calice_de_jeunesse);


    ennemi Brigand = {"Brigand", 1, 3, 3, 9, 0};
    ennemi Squelette = {"Squelette", 2, 6, 4, 13, 10};
    ennemi Goule = {"Goule", 3, 8, 8, 16, 20};
    ennemi Gargouille = {"Gargouille", 4, 9, 10, 18, 25};

    affiche_ennemi(Brigand);
    affiche_ennemi(Squelette);
    affiche_ennemi(Goule); 
    affiche_ennemi(Gargouille);

    int choix1, choix2, choix3;
    printf("Choix du premier combattant: ");
    choix1 = saisie_choix();

    printf("Choix du second combattant: ");
    choix2 = saisie_choix();*/

    //AU NIVEAU 3
    //printf("Choix du troisième combattant: ");
    //choix3 = saisie_choix();

    /*sanitarium(&Boudicca);
    taverne(&Boudicca);
    affiche_personnage(Boudicca);*/


    return 0;
}
