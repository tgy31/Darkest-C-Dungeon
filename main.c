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
    int prix;
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

typedef struct Cellule3{
    ennemi enn;
    struct Cellule3 *suivant;
}Cellule3, *ListeEnnemi;

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

int saisie_choix(int max_el) {//Fonction pour saisir un choix entre 1 et max_el
    int choix;
    int valid = 0;
    
    while (!valid) {

        if (scanf("%d", &choix) == 1) {

            if (choix >= 0 && choix <= max_el) {
                valid = 1;  
            } 
            else {
                printf("Choix invalide. Le nombre doit être entre 1 et %d.\n",max_el - 1);
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

int inserePersonnageTete(ListePers *lst, personnage exemple){//Ajoute un personnage en tete de liste
    ListePers tmp = alloueCellule1(exemple);
    if(tmp != NULL){
        tmp->suivant = *lst;
        *lst = tmp;
        return 1;//SINON
    }
    return 0;//SI ECHEC
}

int insereEnnemiSuite(ListeEnnemi *lst, ennemi exemple){//Ajoute un ennemi en fin de liste
    ListeEnnemi newCell = (Cellule3 *)malloc(sizeof(Cellule3));
    if(newCell == NULL){
        return 0; //SI ECHEC
    }
    newCell->enn = exemple;
    newCell->suivant = NULL;

    if(*lst == NULL){
        *lst = newCell;
        return 1;//SINON
    }
    ListeEnnemi temp = *lst;
    while(temp->suivant != NULL){
        temp = temp->suivant;
    }
    temp->suivant = newCell;
    return 1;//SINON
}

int supprimeTeteEnnemi(ListeEnnemi *lst){//Supprime le premier ennemi de la liste
    if(*lst == NULL){
        return 0; //LISTE VIDE
    }
    ListeEnnemi temp = *lst;
    *lst = temp->suivant;
    free(temp);
    return 1;
}



int inserePersonnageSuite(ListePers *lst, personnage exemple){//Ajoute un personnage en fin de liste
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

int insereAccessoire(ListeAccess *lst, accessoire exemple) {//Ajoute un accessoie en tete de liste
    ListeAccess tmp = alloueCellule2(exemple);
    if (tmp == NULL) {
        return 0; // Échec de l'allocation
    }
    tmp->suivant = *lst; 
    *lst = tmp;          
    return 1;            
}


int supprimePersonnage(ListePers *lst, char nom[]){//Supprime un personnage de la liste
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

int supprimeAccessoire(ListeAccess *lst, char nom[]) {//Supprime un accessoire de la liste
    if (*lst == NULL) {
        return 0; // Liste vide
    }
    ListeAccess temp = *lst, prev = NULL;

    if (strcmp(temp->access.nom, nom) == 0) {
        *lst = temp->suivant;
        free(temp);
        return 1;
    }

    while (temp != NULL && strcmp(temp->access.nom, nom) != 0) {
        prev = temp;
        temp = temp->suivant;
    }

    if (temp == NULL) {
        return 0; // Pas trouvé
    }

    prev->suivant = temp->suivant;
    free(temp);
    return 1;
}

void affiche_lst_accesoire(ListeAccess accesoir){//Affiche la liste des accessoires
    int acc = 0;
    for(;accesoir != NULL;accesoir = accesoir->suivant){
        printf("%d: ", acc);
        affiche_accesoire(accesoir->access);
        acc++;
    }
}

    
void sanitarium(ListePers *personnages) {//Fonction pour soigner les personnages
    srand(time(NULL)); 
    ListePers temp = *personnages;
    while (temp != NULL) {
        int chance = rand() % 5; 
        if ((temp->perso.HP + 5 + chance) > temp->perso.classe.HPmax) {
            int augmentation = temp->perso.classe.HPmax - temp->perso.HP;
            printf("\n%s est passé au sanitarium! +%d HP (HP max atteint)!\n", temp->perso.nom, augmentation);
            temp->perso.HP = temp->perso.classe.HPmax; 
        } else {
            temp->perso.HP += 5 + chance; 
            printf("\n%s est passé au sanitarium! +%d HP!\n", temp->perso.nom, 5 + chance);
        }
        temp = temp->suivant; 
    }
}

void taverne(ListePers *personnages) {//Fonction pour réduire le stress des personnages
    ListePers temp = *personnages;
    while (temp != NULL) {
        if (temp->perso.str - 25 < 0) {
            printf("\n%s est passé à la taverne! -%d stress (stress réduit à 0)!\n", 
                   temp->perso.nom, temp->perso.str);
            temp->perso.str = 0; 
        } else {
            temp->perso.str -= 25; 
            printf("\n%s est passé à la taverne! -25 stress!\n", temp->perso.nom);
        }
        temp = temp->suivant; 
    }
}



void afficheListePersoRec(ListePers lst){//Affiche la liste des personnages
    if(lst != NULL){
        printf("Nom: %s\n", lst->perso.nom);
        afficheListePersoRec(lst->suivant);
    }
}

void afficheListePerso(ListePers lst) {//Affiche la liste des personnages
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

void libererMem3(ListeEnnemi lst){
    Cellule3* courant = lst;
    while (courant != NULL) {
        Cellule3* temp = courant;         
        courant = courant->suivant;      
        free(temp);                      
    }
}


int nb_el_list_perso(ListePers lst) {//DOnne la longeur d'une liste de personnages
    int acc = 0;
    while (lst != NULL) {
        acc++;
        lst = lst->suivant;
    }
    return acc;
}

int nb_el_list_access(ListeAccess lst) {//Donne la longeur d'une liste d'accesoires
    int acc = 0;
    while (lst != NULL) {
        acc++;
        lst = lst->suivant; 
    }
    return acc;
}

void ajout_stat(ListePers *combattants){//Ajoute les bonus des accessoires aux stats des personnages
    ListePers temp_combattants = *combattants;
    while(temp_combattants != NULL){
        if (strlen(temp_combattants->perso.acces.nom) > 0){
            temp_combattants->perso.classe.att += temp_combattants->perso.acces.attbonus;
            temp_combattants->perso.classe.def += temp_combattants->perso.acces.defbonus;
            temp_combattants->perso.classe.HPmax += temp_combattants->perso.acces.HPbonus;
            temp_combattants->perso.classe.rest += temp_combattants->perso.acces.restbonus;
        }
        temp_combattants = temp_combattants->suivant;
    }
}
void remettreAccessoiresDisponibles(ListePers *combattants, ListeAccess *accessoires_disponibles) {//Remet les accessoires des personnages dans la liste des accessoires disponibles
    ListePers temp_combattants = *combattants;

    while (temp_combattants != NULL) {
        if (strlen(temp_combattants->perso.acces.nom) > 0) { // Si un accessoire est équipé
            insereAccessoire(accessoires_disponibles, temp_combattants->perso.acces); // Remettre l'accessoire dans la liste
            printf("Accessoire %s de %s remis dans la liste des accessoires disponibles.\n",
                   temp_combattants->perso.acces.nom, temp_combattants->perso.nom);
            
            // Réinitialiser l'accessoire du personnage
            temp_combattants->perso.acces = (accessoire){"", 0, 0, 0, 0, 0};
        }
        temp_combattants = temp_combattants->suivant; // Passer au personnage suivant
    }
}

void attribuerAccessoires(ListePers *combattants, ListeAccess *accessoires_disponibles) {//Attribue un accessoire à chaque personnage
    printf("\n--- Attribution des accessoires (1 maximum par personnage) ---\n");

    // Afficher les accessoires disponibles
    printf("\nAccessoires disponibles :\n");
    affiche_lst_accesoire(*accessoires_disponibles);

    // Parcourir les combattants pour leur attribuer un accessoire
    ListePers temp_combattants = *combattants;
    while (temp_combattants != NULL) {
        if(*accessoires_disponibles == NULL)break;
        printf("\nAttribuez un accessoire à %s (entrez le numéro ou N pour terminer) :\n", temp_combattants->perso.nom);
        
        char choix[5];
        scanf("%s", choix);

        if (strcmp(choix, "N") == 0) {
            printf("Aucun accessoire attribué à %s.\n", temp_combattants->perso.nom);
        } else {
            int index = atoi(choix) - 1; // Convertir le choix en index (base 0)
            ListeAccess temp_access = *accessoires_disponibles;

            // Parcourir la liste des accessoires pour trouver celui choisi
            for (int k = 0; k < index; k++) {
                if (temp_access == NULL) {
                    printf("Choix invalide.\n");
                    break;
                }
                temp_access = temp_access->suivant;
            }

            if (temp_access != NULL) {
                // Ajouter l'accessoire au personnage
                temp_combattants->perso.acces = temp_access->access;
                printf("%s a équipé %s.\n", temp_combattants->perso.nom, temp_access->access.nom);

                // Supprimer l'accessoire de la liste des accessoires disponibles
                supprimeAccessoire(accessoires_disponibles, temp_access->access.nom);
            }
        }

        // Passer au prochain combattant
        temp_combattants = temp_combattants->suivant;
    }

    printf("\n--- Fin de l'attribution des accessoires ---\n");
}

personnage* choisirCombattant(ListePers lst, int choix) {//Choisir un personnage dans la liste
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

void heal (ListePers *combattant,int id_pers,int vie){//Soigne un personnage
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
    //Attaque du monstre
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
    //Action effectuer par le joueur concerner
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


int deroulement_combat (ListePers *combattant,ennemi *ops,ListeAccess *accessoires_disponibles){
    //Deroulement du combat Tour par tour
    int game = 0;
    srand(time(NULL));
    int start = rand() %2; // 1 pour ennemi et 0 pour les combattant 
    ajout_stat(combattant);
    printf("Le combat commence !\n");

    while (game == 0) {
        if (start == 0) {
            // Tour des personnages
            ListePers tmp = *combattant;
            int stress_count = 0;
            while (tmp != NULL) {
                if (tmp->perso.str >= 100) {
                    printf("\n%s est trop stressé pour agir !\n", tmp->perso.nom);
                    stress_count++;
                }
                else {
                    printf("\nAction pour %s (A: Attaque, D: Défense, R: Soins) : ", tmp->perso.nom);
                    char action;
                    scanf(" %c", &action); // Saisie de l'action
                    Action_combat(&tmp->perso, combattant, action, ops);
                    

                    if (ops->HP_enn <= 0) {
                        printf("\nL'ennemi %s a été vaincu !\n", ops->nom);
                        game = 1; // Victoire
                        break;
                        }
                }

                tmp = tmp->suivant;
            }
            // Si tous les personnages sont stressés, défaite
            if (stress_count == nb_el_list_perso(*combattant)) {
                printf("\nTous vos personnages sont trop stressés pour continuer. Vous avez perdu !\n");
                game = -1;
                break;
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
    remettreAccessoiresDisponibles(combattant,accessoires_disponibles);
    return game; //retourne l'état final ,1 = victoire, -1 = défaite
}

char saisie_choix_oui_non() {//Fonction pour saisir un choix entre O et N
    char choix;
    int valid = 0;
    while (!valid) {
        scanf(" %c", &choix);
        if (choix == 'O' || choix == 'N') {
            valid = 1;
        } else {
            printf("Choix invalide. Entrez O pour oui ou N pour non.\n");
        }
    }
    return choix;
}

accessoire* choisirAccessoire(ListeAccess lst, int choix) {//Choisir un accessoire dans la liste
    int index = 0;
    while (lst != NULL) {
        if (index == choix) {
            printf("Vous avez choisi l'accessoire %s.\n", lst->access.nom);
            return &lst->access; // retourne un pointeur vers l'accessoire
        }
        lst = lst->suivant;
        index++;
    }
    return NULL; // retourne NULL 
}

void affiche_prix_accessoire(ListeAccess lst){//Affiche le prix des accessoires d'une liste
    int acc = 0;
    for(;lst != NULL;lst = lst->suivant){
        printf("%d: ", acc);
        printf("Nom: %s\n", lst->access.nom);
        printf("Prix: %d\n", lst->access.prix);
        acc++;
    }
}

void roulotte(ListeAccess *accessoire_joueur, ListeAccess *roulotte, int *or_courant){//Fonction pour acheter des accessoires
    printf("\n--- Roulotte ---\n");
    printf("Vous avez %d or.\n", *or_courant);
    printf("Accessoires disponibles :\n");
    affiche_prix_accessoire(*roulotte);

    printf("Voulez-vous acheter un accessoire ? (O/N) : ");
    char choix = saisie_choix_oui_non();

    if (choix == 'O') {
        printf("Entrez le numéro de l'accessoire à acheter : ");
        int num_accessoire = saisie_choix(nb_el_list_access(*roulotte));

        accessoire* accessoire_achete = choisirAccessoire(*roulotte, num_accessoire);
        if (accessoire_achete == NULL) {
            printf("Accessoire invalide !\n");
            return;
        }

        if (*or_courant < accessoire_achete->prix) {
            printf("Vous n'avez pas assez d'or pour acheter cet accessoire.\n");
            return;
        }

        insereAccessoire(accessoire_joueur, *accessoire_achete);
        *or_courant -= accessoire_achete->prix;
        supprimeAccessoire(roulotte, accessoire_achete->nom);

        printf("Vous avez acheté l'accessoire %s pour %d or.\n", accessoire_achete->nom, accessoire_achete->prix);
        printf("Il vous reste %d or.\n", *or_courant);
    }
}

void mise_en_place(ListePers *personnages_disponibles, ListePers *combattants, ListeAccess *accessoires_disponibles, ennemi *adversaire, int or_courant, ListeAccess* roul) {
    int nb_combattant = 3;
    if (nb_el_list_perso(*personnages_disponibles)>3)nb_combattant = 3;
    else{
    nb_combattant = nb_el_list_perso(*personnages_disponibles);
    }

    printf("Initilisation du combat ! \n");

    afficheListePerso(*personnages_disponibles);

    printf("\n\n--- Vous avez %d or ---\n\n", or_courant);

    printf("\nChoisissez jusqu'à %d combattants pour le combat.\n", nb_combattant);
    for (int i = 0;i < nb_combattant;i++ ){
        printf("Choix d'un personnage veulliez entre le numero de votre combattant ou n pour terminer: ");
        char choix[10];
        scanf("%s",choix);
        if (strcmp(choix, "N") == 0) {
            break; // l'utilisateur ne veut plus de combattant
        }
        int emplacement = atoi(choix)-1; //Permet d'obtenir un entier pour l'index 
        personnage *combattant_choisi = choisirCombattant(*personnages_disponibles, emplacement);//Renvoi le combattant choisi

        if(combattant_choisi == NULL){
            printf("choix impossible \n");
            i--;//Permet de repeter le choix du joueur
        }
        else{
            inserePersonnageTete(combattants, *combattant_choisi);
            supprimePersonnage(personnages_disponibles, combattant_choisi->nom);
            printf("votre combattant a etait ajouter !\n");
        }
    }

    //2. Roulotte
    roulotte(accessoires_disponibles, roul, &or_courant);
    attribuerAccessoires(combattants,accessoires_disponibles);

     // 3. Configurer l'ennemi
    printf("\nConfiguration de l'ennemi...\n");
    // Exemple simple de configuration de l'ennemi
    /*adversaire = (ennemi){"Brigand", 1, 10, 5, 50, 10}; // Exemple d'ennemi*/

    printf("\n--- Récapitulatif du combat ---\n");
    printf("Combattants :\n");
    afficheListePerso(*combattants);
    printf("\nEnnemi :\n");
    affiche_ennemi(*adversaire);

    printf("\nCombat prêt à commencer !\n");
}

void transferePerso(ListePers *src, ListePers *dest){//Transfere les personnages d'une liste à une autre
    while(*src != NULL){
        if((*src)->perso.HP > 0){
            inserePersonnageTete(dest,(*src)->perso);
        }
        ListePers temp = *src;
        *src = (*src)->suivant;
        free(temp);
    }
    
}
void transfereUnPerso(ListePers *src, ListePers *dest, const char *nom) {//Transfère un personnage d'une liste à une autre
    ListePers current = *src;
    ListePers prev = NULL;
    while (current != NULL) {
        if (strcmp(current->perso.nom, nom) == 0) {
            inserePersonnageTete(dest, current->perso);

            if (prev == NULL) {
                *src = current->suivant;
            } else {
                prev->suivant = current->suivant;
            }
            free(current);
            return; 
        }
        prev = current;
        current = current->suivant;
    }
    printf("%s n'a pas été trouvé dans la liste source.\n", nom);
}

void envoie_sanit(ListePers *arriver, ListePers *destination) {//Envoie un personnage au sanitarium
    if (nb_el_list_perso(*destination) >= 2) {
        printf("Le sanitarium est plein.\n");
        return;
    }
    printf("\nPersonnages disponibles :\n");
    afficheListePerso(*arriver);

    printf("Souhaitez-vous envoyer un personnage dans le sanitarium ?\n");
    printf("Si oui veuillez choisir l'emplacement du personnage (1 à %d) ou 0 pour annuler : ", nb_el_list_perso(*arriver));

    int choix = saisie_choix(nb_el_list_perso(*arriver));
    if (choix == 0) {
        printf("Aucun personnage n'a été envoyé au sanitarium.\n");
        return;
    }
    personnage *perso_choisi = choisirCombattant(*arriver, choix - 1); 
    transfereUnPerso(arriver, destination, perso_choisi->nom);
    sanitarium(destination);
    
}

void envoie_taverne(ListePers *arriver, ListePers *destination) {//Envoie un personnage à la taverne
    if (nb_el_list_perso(*destination) >= 2) {
        printf("La taverne est pleine.\n");
        return;
    }

    printf("\nPersonnages disponibles :\n");
    afficheListePerso(*arriver);
    printf("Souhaitez-vous envoyer un personnage à la taverne ?\n");
    printf("Si oui, veuillez choisir l'emplacement du personnage (1 à %d) ou 0 pour annuler : ", nb_el_list_perso(*arriver));

    int choix = saisie_choix(nb_el_list_perso(*arriver));
    if (choix == 0) {
        printf("Aucun personnage n'a été envoyé à la taverne.\n");
        return;
    }

    personnage *perso_choisi = choisirCombattant(*arriver, choix - 1); 
    transfereUnPerso(arriver, destination, perso_choisi->nom);
    taverne(destination);
    

}


personnage* randomPersonnage(ListePers lst) {//Choisir un personnage au hasard dans la liste
    int index = rand() % 3;
    int i = 0;
    while (lst != NULL) {
        if (i == index) {
            return &lst->perso;
        }
        lst = lst->suivant;
        i++;
    }
    return NULL;
}


int main(){
    classe_personnage Furie = {"Furie", 13, 0, 20, 0};
    classe_personnage Vestale = {"Vestale", 3, 0, 20, 10};
    classe_personnage Chasseur_de_primes = {"Chasseur de primes", 7, 3, 25, 3};
    classe_personnage Maitre_chien = {"Maître chien", 10, 6, 17, 5};

    ennemi ops0 = {"Brigand", 1, 10, 3, 15, 25};
    ennemi ops1 = {"Squelette", 2, 13, 5, 20, 45};
    ennemi ops2 = {"Goule", 3, 15, 5, 20, 45};
    ennemi ops3 = {"Gargouille", 4, 18, 8, 30, 55};
    ennemi ops4 = {"Valkyrie", 5, 22, 10, 30, 66};
    ennemi ops5 = {"Bouliste", 6, 25, 5, 40, 77};
    ennemi ops6 = {"Golem", 7, 30, 5, 40, 88};
    ennemi ops7 = {"Geant Royal", 8, 32, 13, 40, 99};
    ennemi ops8 = {"Mega Chevalier", 9, 37, 5, 45, 99};
    ennemi ops9 = {"Roi des squelettes", 10, 40, 15, 50, 100};

    ListeEnnemi liste_ennemi = NULL;
    insereEnnemiSuite(&liste_ennemi,ops0);
    insereEnnemiSuite(&liste_ennemi,ops1);
    insereEnnemiSuite(&liste_ennemi,ops2);
    insereEnnemiSuite(&liste_ennemi,ops3);
    insereEnnemiSuite(&liste_ennemi,ops4);
    insereEnnemiSuite(&liste_ennemi,ops5);
    insereEnnemiSuite(&liste_ennemi,ops6);
    insereEnnemiSuite(&liste_ennemi,ops7);
    insereEnnemiSuite(&liste_ennemi,ops8);
    insereEnnemiSuite(&liste_ennemi,ops9);




    personnage Boudicca = {"Boudicca", Furie, 18, 23, 0, 0};
    personnage Junia = {"Junia", Vestale, 20, 0, 0, 0};
    personnage William = {"William", Maitre_chien, 17, 0, 0, 0};
    personnage Tardif = {"Tardif", Chasseur_de_primes, 25, 0, 0, 0};

    ListePers tous_personnages = NULL;
    inserePersonnageTete(&tous_personnages,Boudicca);
    inserePersonnageTete(&tous_personnages,Junia);
    inserePersonnageTete(&tous_personnages,William);
    inserePersonnageTete(&tous_personnages,Tardif);


    accessoire Pendentif_tranchant = {"Pendentif tranchant", 5, 1, 0, 0, 0};
    accessoire Calice_de_jeunesse = {"Calice de jeunesse", 0, 3, 5, 0, 5};


    accessoire Pendentif_de_vie = {"Pendentif de vie", 0, 0, 10, 0, 0, 0};
    accessoire Pendentif_de_force = {"Pendentif de force", 10, 0, 0, 0, 0, 5};
    accessoire Pendentif_de_defense = {"Pendentif de defense", 0, 10, 0, 0, 0, 30};
    accessoire Pendentif_de_stress = {"Pendentif de stress", 0, 0, 0, 30, 0, 40};

    ListePers combattants = NULL;
    ListePers perso_disponible = NULL;
    ListePers perso_sanit = NULL;
    ListePers perso_taverne = NULL;
    inserePersonnageTete(&perso_disponible,Boudicca);
    inserePersonnageTete(&perso_disponible,Junia);
    inserePersonnageTete(&perso_disponible,William);
    inserePersonnageTete(&perso_disponible,Tardif);
    ListeAccess accessoires_disponibles = NULL;
    insereAccessoire(&accessoires_disponibles,Pendentif_tranchant);
    insereAccessoire(&accessoires_disponibles,Calice_de_jeunesse);
    ListeAccess roulotte = NULL;
    insereAccessoire(&roulotte,Pendentif_de_vie);
    insereAccessoire(&roulotte,Pendentif_de_force);
    insereAccessoire(&roulotte,Pendentif_de_defense);
    insereAccessoire(&roulotte,Pendentif_de_stress);


    int niveau_actuel = 1;
    int or_courant = 0;
    while(niveau_actuel <= 10){
        afficheListePerso(perso_disponible);
        envoie_sanit(&perso_disponible,&perso_sanit);
        envoie_taverne(&perso_disponible,&perso_taverne);
        

        printf("\n\n---   Niveau %d ---\n\n",niveau_actuel);
        mise_en_place(&perso_disponible,&combattants,&accessoires_disponibles, &liste_ennemi->enn, or_courant, &roulotte);
        int resultat = deroulement_combat(&combattants, &liste_ennemi->enn,&accessoires_disponibles);
        if (resultat == 1) {  
            printf("\n\n\nVictoire ! vous avez réussi le niveau %d\n\n\n", niveau_actuel);
            niveau_actuel++;
            if(niveau_actuel%3 == 0){
                printf("Un nouveau personnage est disponible !\n");
                personnage* rand = randomPersonnage(tous_personnages);
                printf("Un nouveau personnage est disponible : %s\n", rand->nom);
                inserePersonnageSuite(&perso_disponible, *rand);
            }
            or_courant += 10;
            supprimeTeteEnnemi(&liste_ennemi);
            transferePerso(&combattants,&perso_disponible);
            transferePerso(&perso_sanit,&perso_disponible);
            transferePerso(&perso_taverne,&perso_disponible);
        } else if (resultat == -1) {
            printf("Défaite... Vous ferez mieux la prochaine fois.\n");
            break;
        }
    }
    libererMem1(combattants);
    libererMem1(perso_disponible);
    libererMem1(perso_sanit);
    libererMem1(perso_taverne);
    libererMem1(tous_personnages);
    libererMem2(accessoires_disponibles);
    libererMem2(roulotte);
    libererMem3(liste_ennemi);


    
    if(niveau_actuel == 11){
        printf("Vous avez réussi tous les niveaux ! Félicitations !\n");
        return 1;//VICTOIRE
    }
    return 0;//DEFAITE
}
