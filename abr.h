#ifndef ABR
#define ABR

/* includes */

#include "liste.h"
#include "ligne.h"

/* types */

#include "abr_type.h"

/* fonctions de manipulation d abr de stations*/

Un_nabr *construire_abr(Un_elem *liste_sta);

void detruire_abr(Un_nabr *abr);

Un_truc *chercher_station(Un_nabr *abr, char *nom);

#endif
