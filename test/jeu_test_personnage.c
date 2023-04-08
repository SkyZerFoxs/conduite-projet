#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <personnage.h>

#define MAXPV 0
#define ATK 1
#define DEF 2

// Fonction d'initialisation de CUnit
int init_suite(void) {
    return 0;
}

// Fonction de nettoyage de CUnit
int clean_suite(void) {
    return 0;
}

// Test d'amélioration de la statistique maxPv
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

// Test d'amélioration de la statistique attaque
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

// Test d'amélioration de la statistique défense
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

// Test d'amélioration d'une statistique invalide
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
