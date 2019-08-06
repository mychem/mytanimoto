--
-- This SQL script creates the tanimoto function
--
USE mysql;

DROP FUNCTION IF EXISTS tanimoto;
CREATE FUNCTION tanimoto RETURNS REAL SONAME "lib_mytanimoto.so";
