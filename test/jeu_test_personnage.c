#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <personnage.h>

#define MAXPV 0
#define ATK 1
#define DEF 2

/**
 * \file jeu_test_personnage.c
 * \brief Jeu de test sur la fonction upgrade_perso()
 * \author Yamis MANFALOTI
 * \version 1.0
 * \date 04 mars 2023
 *
 * \n Jeu de test: 
 * \n => Correct:
 * \n    - Upgrade stats 0 (  maxPV  )
 * \n    - Upgrade stats 1 ( attaque )
 * \n    - Upgrade stats 2 ( defense )
 * \n => Incorrect:
 * \n    - Upgrade stats 3 ( stats > 2 = invalide )
 */

/**
 * \fn int init_suite(void)
 * \brief Fonction d'initialisation de CUnit
 * \param void Aucun parametre
 * \return int
*/
int init_suite(void) {
    return 0;
}

/**
 * \fn int clean_suite(void)
 * \brief Fonction de nettoyage de CUnit
 * \param void Aucun parametre
 * \return int
*/
int clean_suite(void) {
    return 0;
}

/**
 * \fn void test_upgrade_maxPv(void)
 * \brief Test d'amélioration de la statistique maxPv
 * \param void Aucun parametre
 * \return Aucun retour en fin de fonction
*/
void test_upgrade_maxPv(void) {
    // Création d'un personnage de test
    personnage_t * perso = creer_personnage("Alice");

    // Test d'amélioration de la statistique maxPv
    int maxPv_before = perso->caract->maxPv;
    int result = upgrade_perso(perso, MAXPV);
    int maxPv_after = perso->caract->maxPv;
    CU_ASSERT_EQUAL_FATAL(maxPv_before + UPGRADE_MAX_PV, maxPv_after);
    CU_ASSERT_EQUAL_FATAL(result, 0);

    // Destruction du personnage de test
    supprimer_perso(&perso);
}

/**
 * \fn void test_upgrade_atk(void)
 * \brief Test d'amélioration de la statistique attaque
 * \param void Aucun parametre
 * \return Aucun retour en fin de fonction
*/
void test_upgrade_atk(void) {
    // Création d'un personnage de test
    personnage_t * perso = creer_personnage("Bob");

    // Test d'amélioration de la statistique attaque
    int atk_before = perso->caract->atk;
    int result = upgrade_perso(perso, ATK);
    int atk_after = perso->caract->atk;
    CU_ASSERT_EQUAL_FATAL(atk_before + UPGRADE_ATK, atk_after);
    CU_ASSERT_EQUAL_FATAL(result, 0);

    // Destruction du personnage de test
    supprimer_perso(&perso);
}

/**
 * \fn void test_upgrade_def(void)
 * \brief Test d'amélioration de la statistique défense
 * \param void Aucun parametre
 * \return Aucun retour en fin de fonction
*/
void test_upgrade_def(void) {
    // Création d'un personnage de test
    personnage_t * perso = creer_personnage("Charlie");

    // Test d'amélioration de la statistique défense
    int def_before = perso->caract->def;
    int result = upgrade_perso(perso, DEF);
    int def_after = perso->caract->def;
    CU_ASSERT_EQUAL_FATAL(def_before + UPGRADE_DEF, def_after);
    CU_ASSERT_EQUAL_FATAL(result, 0);

    // Destruction du personnage de test
    supprimer_perso(&perso);
}

/**
 * \fn void test_upgrade_invalidStat(void)
 * \brief Test d'amélioration d'une statistique invalide
 * \param void Aucun parametre
 * \return Aucun retour en fin de fonction
*/
void test_upgrade_invalidStat(void) {
    // Création d'un personnage de test
    personnage_t * perso = creer_personnage("Bob");

    // Test d'amélioration d'une statistique invalide
    int maxPv_before = perso->caract->maxPv;
    int result = upgrade_perso(perso, 3);
    int maxPv_after = perso->caract->maxPv;
    CU_ASSERT_EQUAL_FATAL(maxPv_before, maxPv_after);
    CU_ASSERT_EQUAL_FATAL(result, 1);

    // Destruction du personnage de test
    supprimer_perso(&perso);
}


/**
 * \brief Fonction principale du test
 * 
 * \param void Aucun paramètre en entrée 
 * \return Int qui caractérise le statut de la fonction main
 */
int main() {
    // Initialisation de CUnit
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    // Ajout des tests à la suite de tests
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Teste d'amélioration du personnage ", init_suite, clean_suite);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Test d'amélioration de la statistique maxPv", test_upgrade_maxPv) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Test d'amélioration de la statistique attaque", test_upgrade_atk) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Test d'amélioration de la statistique défense", test_upgrade_def) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Test d'amélioration d'une statistique invalide", test_upgrade_invalidStat) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Exécution des tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    // Nettoyage de CUnit
    CU_cleanup_registry();

    return CU_get_error();
}
