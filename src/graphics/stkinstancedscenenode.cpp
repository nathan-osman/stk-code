#include "stkinstancedscenenode.hpp"
#include "graphics/irr_driver.hpp"

STKInstancedSceneNode::STKInstancedSceneNode(irr::scene::IMesh* mesh, ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
    const irr::core::vector3df& position,
    const irr::core::vector3df& rotation,
    const irr::core::vector3df& scale) :
    CMeshSceneNode(mesh, parent, mgr, id, position, rotation, scale)
{
    createGLMeshes();
}

void STKInstancedSceneNode::createGLMeshes()
{
    for (u32 i = 0; i<Mesh->getMeshBufferCount(); ++i)
    {
        scene::IMeshBuffer* mb = Mesh->getMeshBuffer(i);
        GLmeshes.push_back(allocateMeshBuffer(mb));
    }
    isMaterialInitialized = false;
}

void STKInstancedSceneNode::setFirstTimeMaterial()
{
    if (isMaterialInitialized)
        return;
    irr::video::IVideoDriver* driver = irr_driver->getVideoDriver();
    for (u32 i = 0; i<Mesh->getMeshBufferCount(); ++i)
    {
        scene::IMeshBuffer* mb = Mesh->getMeshBuffer(i);
        if (!mb)
            continue;
        video::E_MATERIAL_TYPE type = mb->getMaterial().MaterialType;

        GLMesh &mesh = GLmeshes[i];
        GeometricMaterial GeometricType = MaterialTypeToGeometricMaterial(type);
        ShadedMaterial ShadedType = MaterialTypeToShadedMaterial(type, mesh.textures);
        initvaostate(mesh, GeometricType, ShadedType);
    }
    isMaterialInitialized = true;
}

void STKInstancedSceneNode::addWorldMatrix(const core::matrix4 &mat)
{
    matrixes.push_back(mat);
}

void STKInstancedSceneNode::render()
{
    irr::video::IVideoDriver* driver = irr_driver->getVideoDriver();
    setFirstTimeMaterial();
    AbsoluteTransformation.setTranslation(vector3df(0., 0., 10.));
    driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

    if (irr_driver->getPhase() == SOLID_NORMAL_AND_DEPTH_PASS)
    {
        computeMVP(ModelViewProjectionMatrix);
        computeTIMV(TransposeInverseModelView);

        glUseProgram(MeshShader::ObjectPass1Shader::Program);
        for (unsigned i = 0; i < GLmeshes.size(); i++)
            drawObjectPass1(GLmeshes[i], ModelViewProjectionMatrix, TransposeInverseModelView);
        return;
    }

    if (irr_driver->getPhase() == SOLID_LIT_PASS)
    {
        glUseProgram(MeshShader::ObjectPass2Shader::Program);
        for (unsigned i = 0; i < GLmeshes.size(); i++)
            drawObjectPass2(GLmeshes[i], ModelViewProjectionMatrix, core::matrix4::EM4CONST_IDENTITY);
        return;
    }
}