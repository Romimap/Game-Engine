#include "chunkfinalizercomponent.h"

ChunkFinalizerComponent::ChunkFinalizerComponent(GameObject* parent) : Component(parent) {

}

void ChunkFinalizerComponent::Start() {
    std::cout << _parent->_name << " ChunkFinalizerComponent starting... Adding an OctreeRendererComponent..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    OctreeRendererComponent* ORC = new OctreeRendererComponent(_parent);
    ORC->Start();

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << _parent->_name << " ChunkFinalizerComponent done! (in " << ((std::chrono::duration<double>)(end - start)).count() << "s)" << std::endl;
}
