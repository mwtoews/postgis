/**********************************************************************
 * $Id: cu_out_twkb.c 6036 2010-10-03 18:14:35Z pramsey $
 *
 * PostGIS - Spatial Types for PostgreSQL
 * http://postgis.net
 * Copyright 2010 Paul Ramsey <pramsey@cleverelephant.ca>
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU General Public Licence. See the COPYING file.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CUnit/Basic.h"

#include "liblwgeom_internal.h"
#include "cu_tester.h"

/*
** Global variable to hold TWKB strings
*/
char *hex_a;
char *hex_b;

/*
** The suite initialization function.
** Create any re-used objects.
*/
static int init_twkb_in_suite(void)
{
	hex_a = NULL;
	hex_b = NULL;
	return 0;
}

/*
** The suite cleanup function.
** Frees any global objects.
*/
static int clean_twkb_in_suite(void)
{
	if (hex_a) free(hex_a);
	if (hex_b) free(hex_b);
	hex_a = NULL;
	hex_b = NULL;
	return 0;
}


static void cu_twkb_in(char *wkt)
{
	LWGEOM_PARSER_RESULT pr;
	LWGEOM *g_a, *g_b;
	uint8_t *twkb_a, *twkb_b;
	size_t twkb_size_a, twkb_size_b;
	/* int i; char *hex; */
	
	if ( hex_a ) free(hex_a);
	if ( hex_b ) free(hex_b);

	/* Turn WKT into geom */
	lwgeom_parse_wkt(&pr, wkt, LW_PARSER_CHECK_NONE);
	if ( pr.errcode ) 
	{
		printf("ERROR: %s\n", pr.message);
		printf("POSITION: %d\n", pr.errlocation);
		exit(0);
	}

	/* Get the geom */
	g_a = pr.geom;
	
	/* Turn geom into TWKB */
	
	twkb_a =lwgeom_to_twkb(g_a, 0, &twkb_size_a,0,0);

	/* Turn WKB back into geom  */
	g_b = lwgeom_from_twkb(twkb_a, twkb_size_a, LW_PARSER_CHECK_NONE);

	/* Turn geom to WKB again */
	
	twkb_b = lwgeom_to_twkb(g_b, 0, &twkb_size_b,0,0);

	/* Turn geoms into WKB for comparisons */
	hex_a = hexbytes_from_bytes(twkb_a, twkb_size_a);
	hex_b = hexbytes_from_bytes(twkb_b, twkb_size_b);

	/* Clean up */
	lwfree(twkb_a);
	lwfree(twkb_b);
	lwgeom_parser_result_free(&pr);
	lwgeom_free(g_b);
}

static void test_twkb_in_point(void)
{
	cu_twkb_in("POINT(0 0 0 0)");
//	printf("old: %s\nnew: %s\n",hex_a, hex_b);
	CU_ASSERT_STRING_EQUAL(hex_a, hex_b);

	cu_twkb_in("POINT(1 1)");
//	printf("old: %s\nnew: %s\n",hex_a, hex_b);
	CU_ASSERT_STRING_EQUAL(hex_a, hex_b);
}

static void test_twkb_in_linestring(void)
{
	cu_twkb_in("LINESTRING(0 0,1 1)");
	CU_ASSERT_STRING_EQUAL(hex_a, hex_b);

	cu_twkb_in("LINESTRING(0 0 1,1 1 2,2 2 3)");
	CU_ASSERT_STRING_EQUAL(hex_a, hex_b);
}

static void test_twkb_in_polygon(void)
{
	cu_twkb_in("POLYGON((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0))");
	CU_ASSERT_STRING_EQUAL(hex_a, hex_b);

	cu_twkb_in("POLYGON((0 0 0 1,0 1 0 2,1 1 0 3,1 0 0 4,0 0 0 5))");
	CU_ASSERT_STRING_EQUAL(hex_a, hex_b);

	cu_twkb_in("POLYGON((0 0 0 1,0 1 0 2,1 1 0 3,1 0 0 4,0 0 0 5))");
	CU_ASSERT_STRING_EQUAL(hex_a, hex_b);

	cu_twkb_in("POLYGON EMPTY");
	CU_ASSERT_STRING_EQUAL(hex_a, hex_b);
}

static void test_twkb_in_multipoint(void) 
{
	cu_twkb_in("MULTIPOINT(0 0 0,0 1 0,1 1 0,1 0 0,0 0 1)");
	CU_ASSERT_STRING_EQUAL(hex_a, hex_b);
}

static void test_twkb_in_multilinestring(void) {}

static void test_twkb_in_multipolygon(void)
{
	cu_twkb_in("MULTIPOLYGON(((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)),((-1 -1 0,-1 2 0,2 2 0,2 -1 0,-1 -1 0),(0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)))");
	CU_ASSERT_STRING_EQUAL(hex_a, hex_b);
	//printf("old: %s\nnew: %s\n",hex_a, hex_b);
}

static void test_twkb_in_collection(void)
{
	cu_twkb_in("GEOMETRYCOLLECTION(POLYGON((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)),POINT(1 1 1))");
	CU_ASSERT_STRING_EQUAL(hex_a, hex_b);

	cu_twkb_in("GEOMETRYCOLLECTION EMPTY");
	CU_ASSERT_STRING_EQUAL(hex_a, hex_b);

	cu_twkb_in("GEOMETRYCOLLECTION(MULTIPOLYGON(((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0))),POLYGON((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)),POINT(1 1 1),LINESTRING(0 0 0, 1 1 1))");
	CU_ASSERT_STRING_EQUAL(hex_a, hex_b);

}


/*
** Used by test harness to register the tests in this file.
*/
void twkb_in_suite_setup(void);
void twkb_in_suite_setup(void)
{
	CU_pSuite suite = CU_add_suite("twkb_input", init_twkb_in_suite, clean_twkb_in_suite);
	PG_ADD_TEST(suite, test_twkb_in_point);
	PG_ADD_TEST(suite, test_twkb_in_linestring);
	PG_ADD_TEST(suite, test_twkb_in_polygon);
	PG_ADD_TEST(suite, test_twkb_in_multipoint);
	PG_ADD_TEST(suite, test_twkb_in_multilinestring);
	PG_ADD_TEST(suite, test_twkb_in_multipolygon);
	PG_ADD_TEST(suite, test_twkb_in_collection);
}
