#pragma once
#include "Object.h"
#include "Light.h"
#include "Camera.h"

class Scene {
private:
	std::vector<Object*> m_OpaqueObjects;
	std::vector<Object*> m_TransparentObjects;
	std::vector<Light*> m_Lights;
	std::vector<Camera*> m_Cameras;

	std::size_t m_ActiveCamera;

	bool m_ShouldUpdate;
public:
	Scene();
	~Scene();

	void Update(float deltaTime);

	Camera* GetActiveCamera();
	const std::size_t GetActiveCameraIndex() const;
	void SetActiveCamera(std::size_t index);
	void IncrementActiveCamera();
	void DecrementActiveCamera();

	void AddOpaqueObject(Object* object);
	void AddOpaqueObject(Object* object, std::size_t index);
	void AddTransparentObject(Object* object);
	void AddTransparentObject(Object* object, std::size_t index);
	void AddLight(Light* light);
	void AddLight(Light* light, std::size_t index);
	void AddCamera(Camera* camera);
	void AddCamera(Camera* camera, std::size_t index);

	// searches opaque and transparent objects
	// DOES NOT DELETE REMOVED OBJECT
	Object* RemoveObject(Object* object);
	// DOES NOT DELETE REMOVED OBJECT
	Object* RemoveOpaqueObject(Object* object);
	// DOES NOT DELETE REMOVED OBJECT
	Object* RemoveTransparentObject(Object* object);
	// DOES NOT DELETE REMOVED OBJECT
	Light* RemoveLight(Light* light);
	// DOES NOT DELETE REMOVED OBJECT
	Camera* RemoveCamera(Camera* camera);

	// does not check bounds
	// DOES NOT DELETE REMOVED OBJECT
	Object* RemoveOpaqueObject(std::size_t index);
	// does not check bounds
	// DOES NOT DELETE REMOVED OBJECT
	Object* RemoveTransparentObject(std::size_t index);
	// does not check bounds
	// DOES NOT DELETE REMOVED OBJECT
	Light* RemoveLight(std::size_t index);
	// does not check bounds
	// DOES NOT DELETE REMOVED OBJECT
	Camera* RemoveCamera(std::size_t index);

	// checks bounds and returns nullptr if out of bounds
	// DOES NOT DELETE REMOVED OBJECT
	Object* RemoveOpaqueObjectAt(std::size_t index);
	// checks bounds and returns nullptr if out of bounds
	// DOES NOT DELETE REMOVED OBJECT
	Object* RemoveTransparentObjectAt(std::size_t index);
	// checks bounds and returns nullptr if out of bounds
	// DOES NOT DELETE REMOVED OBJECT
	Light* RemoveLightAt(std::size_t index);
	// checks bounds and returns nullptr if out of bounds
	// DOES NOT DELETE REMOVED OBJECT
	Camera* RemoveCameraAt(std::size_t index);

	// does not check bounds
	// DELETES THE OBJECT
	void DeleteOpaqueObject(std::size_t index);
	// does not check bounds
	// DELETES THE OBJECT
	void DeleteTransparentObject(std::size_t index);
	// does not check bounds
	// DELETES THE OBJECT
	void DeleteLight(std::size_t index);
	// does not check bounds
	// DELETES THE OBJECT
	void DeleteCamera(std::size_t index);

	// checks bounds and returns nullptr if out of bounds
	// DELETES THE OBJECT
	void DeleteOpaqueObjectAt(std::size_t index);
	// checks bounds and returns nullptr if out of bounds
	// DELETES THE OBJECT
	void DeleteTransparentObjectAt(std::size_t index);
	// checks bounds and returns nullptr if out of bounds
	// DELETES THE OBJECT
	void DeleteLightAt(std::size_t index);
	// checks bounds and returns nullptr if out of bounds
	// DELETES THE OBJECT
	void DeleteCameraAt(std::size_t index);

	// does not check bounds
	Object* GetOpaqueObject(std::size_t index);
	// checks bounds and returns nullptr if out of bounds
	Object* GetOpaqueObjectAt(std::size_t index);
	const std::size_t GetNumOpaqueObjects() const;
	std::vector<Object*>& GetOpaqueObjects();

	// does not check bounds
	void MarkObjectTransparent(std::size_t index);
	// checks bounds and does nothing if out of bounds
	void MarkObjectTransparentAt(std::size_t index);
	// searches for opaque object and marks transparent if it can find it
	void MarkObjectTransparent(Object* obj);

	// does not check bounds
	void MarkObjectOpaque(std::size_t index);
	// checks bounds and does nothing if out of bounds
	void MarkObjectOpaqueAt(std::size_t index);
	// searches for opaque object and marks transparent if it can find it
	void MarkObjectOpaque(Object* obj);

	// does not check bounds
	Object* GetTransparentObject(std::size_t index);
	// checks bounds and returns nullptr if out of bounds
	Object* GetTransparentObjectAt(std::size_t index);
	const std::size_t GetNumTransparentObjects() const;
	std::vector<Object*>& GetTransparentObjects();

	// does not check bounds
	Camera* GetCamera(std::size_t index);
	// checks bounds and returns nullptr if out of bounds
	Camera* GetCameraAt(std::size_t index);
	const std::size_t GetNumCameras() const;
	std::vector<Camera*>& GetCameras();

	// does not check bounds
	Light* GetLight(std::size_t index);
	// checks bounds and returns nullptr if out of bounds
	Light* GetLightAt(std::size_t index);
	const std::size_t GetNumLights() const;
	std::vector<Light*>& GetLights();

	// x and y are the location in the viewport to render the scene
	// width and height are the size
	void Render(int x, int y, int width, int height);
};