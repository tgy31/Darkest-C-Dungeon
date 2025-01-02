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
    int attbonus;
    int defbonus;
    int HPbonus;
    int restbonus;
    int strred;
}accessoire;


typedef struct{
    char nom[20];  
    classe_personnage classe;
    int HP;
    int str;
    int nbcomb;
    int defense_bonus;
    accessoire acces;
}personnage;

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

int saisie_choix(int max_el) {//Pour quelles cas selection de personnage ou attaque ?
    int choix;
    int valid = 0;
    
    while (!valid) {

        if (scanf("%d", &choix) == 1) {

            if (choix >= 1 && choix <= max_el) {
                valid = 1;  
            } 
            else {
                printf("Choix invalide. Le nombre doit être entre 1 et %d.\n",max_el);
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

int supprimePersonnage(ListePers *lst, char nom[]){//Faudrais ajouter la supprions d'accesoir en meme temp que le personnage
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

/*void roulotte(){

}*/

void afficheListePersoRec(ListePers lst){
    if(lst != NULL){
        printf("Nom: %s\n", lst->perso.nom);
        afficheListePersoRec(lst->suivant);
    }
}

void afficheListePerso(ListePers lst) {
    while (lst != NULL) {  // Parcourt la liste tant qu'il y a des éléments
        printf("Nom: %s\n", lst->perso.nom);
        lst = lst->suivant;  // Avance au suivant
    }
    printf("\nfin affichage personage\n\n");
}

void libererMem1(ListePers lst){//Clear list
    Cellule1* courant = lst;
    while (courant != NULL) {
        Cellule1* temp = courant;         
        courant = courant->suivant;      
        free(temp);                      
    }
}

void libererMem2(ListeAccess lst){//Clear list
    Cellule2* courant = lst;
    while (courant != NULL) {
        Cellule2* temp = courant;         
        courant = courant->suivant;      
        free(temp);                      
    }
}


int nb_el_list_perso(ListePers lst) {//DOnne la longeur de la liste pour les personnage
    int acc = 0;
    while (lst != NULL) {
        acc++;
        lst = lst->suivant;
    }
    return acc;
}

int nb_el_list_access(ListeAccess lst) {//Donne la longeur de la liste pour les accesoires
    int acc = 0;
    while (lst != NULL) {
        acc++;
        lst = lst->suivant; 
    }
    return acc;
}


personnage* choisirCombattant(ListePers lst, int choix) {
    int index = 0;
    while (lst != NULL) {
        if (index == choix) {
            return &lst->perso; // retourne un pointeur vers le personnage
        }
        lst = lst->suivant;
        index++;
    }
    return NULL; // retourne NULL 
}

void heal (ListePers *combattant,int id_pers,int vie){
    personnage *personnage_heal = choisirCombattant(*combattant,id_pers);
     if (personnage_heal == NULL) {
        printf("Personnage invalide !\n");
        return;
    }
    if (personnage_heal->HP + vie > personnage_heal->classe.HPmax )
        personnage_heal->HP = personnage_heal->classe.HPmax;//bloquer over heal
    else {
        personnage_heal->HP += vie;//Heal du personnage choisi
        }
     printf("%s a été soigné de %d HP, il a maintenant %d/%d HP.\n",
           personnage_heal->nom, vie, personnage_heal->HP, personnage_heal->classe.HPmax);
}


void monstre_combat(ListePers *combattant,ennemi *ops){
    if (*combattant == NULL) {
        printf("Aucun combattant disponible.\n");
        return;
    }
    srand(time(NULL));
    int id_personnage = rand() %nb_el_list_perso(*combattant);//choix au hasard du personnage qui vas etre attaquer
    int choix_ops = rand() %2; // 1 pour stress et 0 pour phyisque
    float roll = 0.8 + ((float)rand() / (float)RAND_MAX) * 0.4; // Intervalle [0.8;1.2]
    personnage *pers_subit = choisirCombattant(*combattant,id_personnage);//declration du joueur focus par l'ennemi

    if (ops->HP_enn <= 0) {
        printf("L'ennemi %s est déjà vaincu.\n", ops->nom);
        return;
    }

    if (pers_subit == NULL) {
        printf("Erreur : personnage invalide !\n");
        return;
    }

    if(choix_ops == 1){
        if(pers_subit->str >=100){
            pers_subit->HP -= (ops->att_enn - (pers_subit->defense_bonus + pers_subit->classe.def))*roll; 
        }
        else{
            printf("L'ennemi choisi une attaque de stress!\n");
            pers_subit->str += (ops->attstr_enn-pers_subit->acces.strred)*roll;
            printf("L'ennemi attaque de stress %s pour %f HP. Il monte à %d stress.",
            pers_subit->nom,pers_subit->str + ((ops->attstr_enn-pers_subit->acces.strred)*roll), pers_subit->str);
        }
    } 
    else {
        printf("L'ennemi choisi une attaque physique!\n");
        pers_subit->HP -= (ops->att_enn - (pers_subit->defense_bonus + pers_subit->classe.def))*roll;

        printf("L'ennemi attaque %s pour %f HP. Il descend à %d HP.",
        pers_subit->nom,(ops->att_enn - (pers_subit->defense_bonus + pers_subit->classe.def))*roll, pers_subit->HP); 
    }

    if(pers_subit->HP <= 0){
        printf("%s est mort !\n", pers_subit->nom);
        supprimePersonnage(combattant,pers_subit->nom);
        
    }
   
}

void Action_combat(personnage *pers_actuelle,ListePers *combattant,char action,ennemi *ops){
    if (pers_actuelle->str >= 100 ||pers_actuelle->HP <=0 ){//bloque le tour du joueur a cause du stress
        printf("Votre a sauter car vous etez trop stresser !");
        return;
        }

    float roll = 0.8 + ((float)rand() / (float)RAND_MAX) * 0.4; // Intervalle [0.8;1.2]
    int pv_ops_actu = ops->HP_enn;

    if (action == 'A'){
        if ((pers_actuelle->classe.att - ops->def_enn)>0){ ops->HP_enn -= ((pers_actuelle->classe.att - ops->def_enn)*roll);}
        else {ops->HP_enn -=  1;}
        printf("Dommages à l'ennemi : %d HP, %d HP restants",pv_ops_actu-ops->HP_enn,ops->HP_enn);
    }
    if (action == 'D'){
        pers_actuelle->defense_bonus = 0 ;
        pers_actuelle->defense_bonus += (pers_actuelle->classe.def * 0.1);
    }
    if (action == 'R'){
        printf("Quelle personnage souhaitez-vous soigner:\n");
        afficheListePersoRec(*combattant);
        int taille_list = nb_el_list_perso(*combattant);
        int choix_pers = saisie_choix(taille_list);
        heal(combattant,choix_pers,pers_actuelle->classe.rest);
    }
}


int deroulement_combat (ListePers *combattant,ennemi *ops){
    int game = 0;
    srand(time(NULL));
    int start = rand() %2; // 1 pour ennemi et 0 pour les combattant 
    printf("Le combat commence !\n");

    while (game == 0) {
        if (start == 0) {
            // Tour des personnages
            ListePers tmp = *combattant;
            while (tmp != NULL) {
                printf("\nAction pour %s (A: Attaque, D: Défense, R: Soins) : ", tmp->perso.nom);
                char action;
                scanf(" %c", &action); // Saisie de l'action
                Action_combat(&tmp->perso, combattant, action, ops);

                if (ops->HP_enn <= 0) {
                    printf("\nL'ennemi %s a été vaincu !\n", ops->nom);
                    game = 1; // Victoire
                    break;
                }

                tmp = tmp->suivant;
            }
            start = 1; // Passe au tour de l'ennemi
        } else {
            // Tour de l'ennemi
            printf("\nTour de l'ennemi %s\n", ops->nom);
            monstre_combat(combattant, ops);

            // Vérifie si tous les combattants sont morts
            if (*combattant == NULL) {
                printf("\nTous vos personnages sont morts. Vous avez perdu !\n");
                game = -1; // Défaite
                break;
            }
            start = 0; // Passe au tour des combattants
        }
    }

    return game; // Retourne l'état final : 1 = victoire, -1 = défaite
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

    ennemi ops = {"Brigand", 1, 10, 5, 50, 100};
    personnage Boudicca = {"Boudicca", Furie, 18, 23, 0, 0};
    personnage Junia = {"Junia", Vestale, 20, 0, 0, 0};
    personnage William = {"William", Maitre_chien, 17, 0, 0, 0};
    personnage Tardif = {"Tardif", Chasseur_de_primes, 25, 0, 0, 0};

    //affiche_personnage(Boudicca);
    //affiche_personnage(Junia); 

    ListePers liste_perso = NULL;
    printf("affichage 1\n");
    afficheListePerso(liste_perso);
    inserePersonnageTete(&liste_perso, Boudicca);
    inserePersonnageTete(&liste_perso, Junia);
    
    int resultat = deroulement_combat(&liste_perso, &ops);

    if (resultat == 1) {
        printf("Victoire !\n");
    } else if (resultat == -1) {
        printf("Défaite...\n");
    }

    libererMem1(liste_perso);
    return 0;

    //int temp = supprimePersonnage(&liste_perso, "Junia");
    //printf("%d\n\n", temp);
    //printf("affichage 3\n");
    //afficheListePerso(liste_perso);



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
