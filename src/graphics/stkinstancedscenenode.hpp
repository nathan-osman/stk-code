#ifndef STKINSTANCEDSCENENODE_HPP
#define STKINSTANCEDSCENENODE_HPP

#include "stkmesh.hpp"

class STKInstancedSceneNode : public irr::scene::CMeshSceneNode
{
protected:
    std::vector<GLMesh> GLmeshes;
    std::vector<core::matrix4> matrixes;
    core::matrix4 ModelViewProjectionMatrix, TransposeInverseModelView;
    void createGLMeshes();
    bool isMaterialInitialized;
    void setFirstTimeMaterial();
public:
    STKInstancedSceneNode(irr::scene::IMesh* mesh, ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
        const irr::core::vector3df& position = irr::core::vector3df(0, 0, 0),
        const irr::core::vector3df& rotation = irr::core::vector3df(0, 0, 0),
        const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f));
    virtual void render();
    void addWorldMatrix(const core::matrix4 &);
};

#endif