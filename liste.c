#include "liste.h"
#include "abr.h"


Un_elem *inserer_deb_liste(Un_elem *liste, Un_truc *truc){
	Un_elem *newliste = malloc(sizeof(Un_elem));
	newliste->truc  = truc;
	newliste -> suiv =liste;
	return newliste;
}





Un_elem *lire_connexions(char *nom_fichier, Une_ligne *liste_ligne, Un_nabr *abr_sta){
	Un_elem *liste=NULL;
	FILE *f= fopen(nom_fichier,"r");
	char buffer[300];
	if(f==NULL){
		fprintf(stderr,"Erreur de lecture de fichier dans lire_connexion \n");
		return NULL;
	}
	Une_coord coord, cdep, carr;
	Tdata data;
	char *t;
	char *code, *nom_dep, *nom_arr;
	float val, dx, dy;
	Un_truc *connex;
	Un_truc *sta_dep;
	
	
	while(fgets (buffer,300,f)){
		
		if(buffer[0]== '#'){continue;}
		
		
		
		t=strtok(buffer,";"); 
		code = strdup(t);
		t = strtok(NULL,";");
		nom_dep = strdup(t);
		t = strtok(NULL,";");
		nom_arr = strdup(t);
		t = strtok(NULL, "\n");
		val = atof(t);
		
		//recherche de la ligne
		
		if((data.con.ligne = chercher_ligne(liste_ligne,code) )==NULL){
			fprintf(stderr,"1Erreur station pas trouvé dans lire_connexion dans \n");
			continue;
		}
		if((data.con.sta_dep = chercher_station(abr_sta,nom_dep))==NULL){
			fprintf(stderr,"2Erreur station pas trouvé dans lire_connexion dans \n");
			continue;
		}
		if((data.con.sta_arr = chercher_station(abr_sta,nom_arr))==NULL){
			fprintf(stderr,"3Erreur station pas trouvé dans lire_connexion dans \n");
			continue;
		}
		
		//Calcule de val si val==0
		
		if(val==0){
			cdep = data.con.sta_dep -> coord;
			carr = data.con.sta_arr -> coord;
			coord.lon = cdep.lon;
			coord.lat = cdep.lat;
			dx = ((carr.lon - cdep.lon)*M_PI*6370*0.70)/180;
			dy = ((carr.lat - cdep.lat)*M_PI*6370)/180;
			val = 60*sqrt(dx*dx + dy*dy)/data.con.ligne -> vitesse;
		}
		
		connex = creer_truc(coord, CON, data, val);
		
		//Ajout a la station de depart
		
		sta_dep = data.con.sta_dep;
		(sta_dep -> data.sta.nb_con) ++;
		sta_dep->data.sta.tab_con=realloc(sta_dep->data.sta.tab_con,sta_dep->data.sta.nb_con * sizeof(Un_truc*));
		sta_dep -> data.sta.tab_con [sta_dep -> data.sta.nb_con - 1] = connex;
		
		// Realloc copie tout de malloc et ajoute de la memoire pour Un_truc
		// Ajout a la liste des connexions
		
		liste = inserer_deb_liste (liste, connex);
		
	}
	
	fclose(f);
	return liste;
}



		


Un_elem *inserer_liste_trie(Un_elem *liste, Un_truc *truc){
	
	Un_elem *pe, *liste_ini = liste;
	
	pe= (Un_elem *)malloc(sizeof(Un_elem));
	if(pe == NULL){
		fprintf(stderr, "Erreur d allocation dans inserer_list_trie \n");
		return liste;
	}
	
	pe -> truc = truc;
	pe -> suiv = NULL;
	if(liste == NULL){return pe;}
	if(liste-> truc -> user_val > truc -> user_val){
		pe -> suiv = liste;
		return pe;
	}
	
	while(liste -> suiv){
		if(liste -> suiv -> truc -> user_val > truc -> user_val){
			break;
		}
		liste = liste -> suiv;
	}
		
	pe -> suiv = liste -> suiv;
	liste -> suiv = pe;
	return liste_ini;
}


void ecrire_liste(FILE *flux, Un_elem *liste){
	while(liste){
		switch(liste -> truc ->type){
			case STA:
			fprintf(flux ,"%f,%f,%s \n", 
				liste->truc->coord.lon,
				liste->truc->coord.lat,
				liste->truc->data.sta.nom);
			break;
			case CON:
			fprintf(flux,"%s,%s,%s,%f \n",
				liste -> truc -> data.con.ligne -> code,
				liste -> truc -> data.con.sta_dep ->data.sta.nom,
				liste -> truc -> data.con.sta_arr ->data.sta.nom,
				liste ->truc -> user_val);
				break;
				default:
				fprintf(stderr, "Erreur dans ecrire_liste: truc inconnu. \n");
				}
			liste =liste ->suiv;
		}
}


/*Itérative*/

void detruire_liste(Un_elem *liste){
	while(liste){
		Un_elem *next = liste -> suiv;
		free(liste);
		liste = next;
		}
}

void detruire_liste_et_truc(Un_elem *liste){
	while(liste){
		Un_elem *next = liste -> suiv;
		free(liste -> truc);
		free(liste);
		liste = next;
		}
}

/*Recursive*/

void detruire_liste2(Un_elem *liste){
	if(liste==NULL){
		return;
	}
	detruire_liste2(liste -> suiv);
	free(liste);
}

void detruire_liste_et_truc3(Un_elem *liste){
	if(liste == NULL){
		return;
	}
	detruire_liste_et_truc3(liste -> suiv);
	free(liste -> truc);
	free(liste);
}


/*Mixte et Autre*/

void detruire_liste_et_truc2(Un_elem *liste){
	while(liste){
		Un_elem *next = liste -> suiv;
		free(liste -> truc);
		liste = next;
		}
	detruire_liste(liste);
}


void detruire_liste_et_truc4(Un_elem *liste){
	while(liste){
		Un_elem *next = liste -> suiv;
		free(liste -> truc);
		liste = next;
		}
	detruire_liste2(liste);
}


Un_elem* lire_stations(char* nom_fichier) {
	FILE* f;
	f = fopen(nom_fichier, "r");
	if(!f) {
		fprintf(stderr, "Probleme d'ouverture dans lire_station \n");
		return NULL;
	}
	char tab[300];
	Un_truc* truc;
	Un_elem* liste;
	char *t;
	Une_coord c;
	Tdata data;
	liste = NULL;
	while(fgets(tab,300,f)) {
		t = strtok(tab, ";");
		c.lon = atof(t);
		t = strtok(NULL, ";");
		c.lat = atof(t);
		t = strtok(NULL, "\n");
		data.sta.nom = strdup(t);
		data.sta.tab_con=NULL;
		data.sta.nb_con=0;
		truc = creer_truc(c, STA, data, c.lat);
		liste = inserer_liste_trie(liste, truc);
	}
	fclose(f);
	return liste;
}


void limites_zone(Un_elem *liste, Une_coord *limite_no, Une_coord *limite_se){
	if(liste==NULL){return;}
	*limite_no=liste->truc->coord;
	*limite_se=liste->truc->coord;
	liste=liste->suiv;
	while(liste){
		if(liste -> truc -> coord.lon < limite_no-> lon){
			limite_no->lon=liste->truc->coord.lon;
		}
		else if(liste->truc->coord.lon > limite_se->lon){
			limite_se->lon=liste->truc->coord.lon;
		}
		if(liste -> truc -> coord.lat > limite_no-> lat){
			limite_no->lat=liste->truc->coord.lat;
		}
		else if(liste->truc->coord.lat < limite_se->lat){
			limite_se->lat=liste->truc->coord.lat;
		}
	liste=liste->suiv;
	}
}


				
