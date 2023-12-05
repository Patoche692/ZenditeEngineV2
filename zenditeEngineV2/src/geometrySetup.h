#pragma once
#include "utils.h"

void GenerateBasicSquareVAO(unsigned int &VAO_Square, unsigned int &VBO_Square, unsigned int &EBO_Square);

void GenerateTexTriVAO(unsigned int &VAO_TexTri, unsigned int &VBO_TexTri, unsigned int& EBO_TexTri);

void GenerateCubeNoEBO(unsigned int& VAO_Cube, unsigned int& VBO_Cube);

void GenerateCubeNoEBONoNormals(unsigned int& VAO_Cube, unsigned int& VBO_Cube);

void bindVao(unsigned int VAO);

void unbindVao();