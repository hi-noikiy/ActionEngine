﻿//****************************
// Tipo da action para armazenar os modelos 3d
// creator: Daniel Gadens
// date: 10/06/2011
//*****************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using EngineMathCLI;

namespace ACFramework.FileStructs
{
    public struct AMT_HEAD
    {
        public uint Version;
        public uint NumVertices;
        public uint NumIndices;
        public uint NumFaces;
        public uint NumMeshes;
        public uint NumMaterials;
        public uint NumJoints;
        public float AnimationFPS;
        public float CurrentTime;
        public uint NumFrames;
        public uint NumAnimations;
        public uint HasSkeleton;
        public uint HasSkin;
    }

    public class AMT_VERTEX
    {
        public Vector3 Position;
        public Vector2 TexCoord1;                 // UV 0 Texturescoor.
        public Vector2 TexCoord2;                 // UV 1 Texturescoor.
        public Vector3 Normal;                    // Normalvector
        public uint[] BoneIndices;
        public float[] BoneWeights;
        public uint Flag;                          // flag para objetivos diversos

        public int SID;    //id original do vertices (nao é exportado) é apenas um auxilio para achar o vertice quando for atualizar o peso dos bones

        public AMT_VERTEX()
        {
            BoneIndices = new uint[4];
            BoneWeights = new float[4];
        }

        public static bool operator ==(AMT_VERTEX v1, AMT_VERTEX v2)
        {
            if ((v1.BoneIndices == null && v2.BoneIndices != null ||
                 v1.BoneIndices != null && v2.BoneIndices == null) ||
                (v1.BoneWeights == null && v2.BoneWeights != null ||
                 v1.BoneWeights != null && v2.BoneWeights == null))
                return false;

            if (v1.BoneIndices != null && v1.BoneWeights != null)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (v1.BoneIndices[i] != v2.BoneIndices[i] ||
                        v1.BoneWeights[i] != v2.BoneWeights[i])
                        return false;
                }
            }

            if (v1.Position != v2.Position ||
                v1.TexCoord1 != v2.TexCoord1 ||
                v1.TexCoord2 != v2.TexCoord2 ||
                v1.Normal != v2.Normal
                )
                return false;

            return (v1.Flag == v2.Flag);
        }

        public static bool operator !=(AMT_VERTEX v1, AMT_VERTEX v2)
        {
            if ((v1.BoneIndices == null && v2.BoneIndices != null ||
                 v1.BoneIndices != null && v2.BoneIndices == null) ||
                (v1.BoneWeights == null && v2.BoneWeights != null ||
                 v1.BoneWeights != null && v2.BoneWeights == null))
                return true;

            if (v1.BoneIndices != null && v1.BoneWeights != null)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (v1.BoneIndices[i] != v2.BoneIndices[i] ||
                        v1.BoneWeights[i] != v2.BoneWeights[i])
                        return true;
                }
            }

            if (v1.Position != v2.Position ||
                v1.TexCoord1 != v2.TexCoord1 ||
                v1.TexCoord2 != v2.TexCoord2 ||
                v1.Normal != v2.Normal)
                return true;

            return (v1.Flag != v2.Flag);
        }

        public static bool EqualsWithoutFlagTest(AMT_VERTEX v1, AMT_VERTEX v2)
        {
            if ((v1.BoneIndices == null && v2.BoneIndices != null ||
                 v1.BoneIndices != null && v2.BoneIndices == null) ||
                (v1.BoneWeights == null && v2.BoneWeights != null ||
                 v1.BoneWeights != null && v2.BoneWeights == null))
                return true;

            if (v1.BoneIndices != null && v1.BoneWeights != null)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (v1.BoneIndices[i] != v2.BoneIndices[i] ||
                        v1.BoneWeights[i] != v2.BoneWeights[i])
                        return false;
                }
            }

            if (v1.Position != v2.Position ||
                v1.TexCoord1 != v2.TexCoord1 ||
                v1.TexCoord2 != v2.TexCoord2 ||
                v1.Normal != v2.Normal)
                return false;

            return true;
        }
    }


    public struct AMT_FACE
    {
        public List<int> Indices;            // Indices
        public Vector3 Normal;              // Normalvector
        public uint MeshID;             // MeshID
        public uint Flag;              // Flags
    }

    public struct AMT_MATERIAL
    {
        public string Name;                           // Name
        public Vector3 Ambient;                       // Color Ambient
        public Vector3 Diffuse;                       // Color Diffuse
        public Vector3 Specular;                      // Color Specular
        public Vector3 Emissive;                      // Color Emissive
        public float SpecularPower;                   // Specular Power
        public float Transparency;                    // Transparency
        public string DiffuseTexture;
        public string SpecularTexture;
        public string NormalTexture;
        public string AnimatedTexture;
        public uint Flag;                             // Flags
    }

    public struct AMT_MESH
    {
        public string Name;                         // Name
        public int NumFaces;                         // Number of Faces
        public List<int> FaceIndices;                     // Indices das faces
        public uint MaterialID;                        // MaterialID
        public uint Flag;                             // Flags
    }

    public class AMT_KF
    {
        public float Time;                               // Time
        public Matrix BindMatrix;                        // Matrix
    }

    public class AMT_JOINT
    {
        public uint ID;                                //Id q ta na lista principal, armazeno para ser facil de achar
        public string Name;
        public int ParentID;                           // ParentID
        public uint NumChildren;                        // Numero de filhos
        public List<uint> JointChildren;               // Ids dos filhos do bone 
        public uint NumKF;                             // Numero de Keyframes
        public List<AMT_KF> KFData;                    // KF Matrix
        public uint IsAnimated;                        // Animiert
        public uint Flag;                              // Flags
        public Matrix BindMatrix;                      // Bind Matrix (joint)
        public Matrix MatrixAbsolute;                  // Matrix absolute
        public Matrix InverseBindMatrix;               // Inverse Bind Matrix (inverse joint)

        //nao é exportado, usado para achar o indice do bone pelo controller do skin
        public string SID;
        //nao é exportado, usando para encontrar as animacoes id do bone / sid da matrix
        public string TARGET;
    }

    public struct AMT_ANIMATION
    {
        public string Name;
        public float StartFrame;
        public float EndFrame;
        public uint Active;
    }

    public struct AMT_MODEL
    {
        public AMT_HEAD Head;                 // Header
        public List<AMT_VERTEX> Vertices;             // Vertices
        public List<AMT_FACE> Faces;                // Faces
        public List<AMT_MESH> Meshes;                // Meshs
        public List<AMT_MATERIAL> Materials;            // Materials
        public List<AMT_JOINT> Joints;               // Joints
        public List<AMT_ANIMATION> Animations;            // Animations
    }
}
