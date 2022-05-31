#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "Pipeline.h"
#include "camera.h"
#include "math_3d.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "util.h"
#include "mesh.h"
#include "shafow_map_fbo.h"
#include "shadow_map_technique.h"

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024

/*struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;
	Vector3f m_normal;

	Vertex() {}

	Vertex(Vector3f pos, Vector2f tex)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = Vector3f(0.0f, 0.0f, 0.0f);
	}
}; */


class Main : public ICallbacks
{
public:

	Main()
	{
		pGameCamera = NULL;
	//	pTexture = NULL;
		pEffect = NULL;
		sc = 0.0f;

		m_pMesh = NULL;
		m_pQuad = NULL;
		m_pShadowMapTech = NULL;
	/*	directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		directionalLight.AmbientIntensity = 0.5f;
		directionalLight.DiffuseIntensity = 0.75f;
		directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0); */
		m_spotLight.AmbientIntensity = 0.0f;
		m_spotLight.DiffuseIntensity = 0.9f;
		m_spotLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_spotLight.Attenuation.Linear = 0.01f;
		m_spotLight.Position = Vector3f(-20.0, 20.0, 5.0f);
		m_spotLight.Direction = Vector3f(1.0f, -1.0f, 0.0f);
		m_spotLight.Cutoff = 20.0f;
	}

	~Main()
	{
		SAFE_DELETE(pEffect);
		SAFE_DELETE(m_pShadowMapTech);
		SAFE_DELETE(pGameCamera);
		SAFE_DELETE(m_pMesh);
		SAFE_DELETE(m_pQuad);
	}

	bool Init()
	{
	/*	Vector3f Pos(0.0f, 0.0f, -3.0f);
		Vector3f Target(0.0f, 0.0f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);
		pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		unsigned int Indices[] = { 0, 3, 1,
								   1, 3, 2,
								   2, 3, 0,
								   1, 2, 0 };

		createIndexBuffer(Indices, sizeof(Indices));
		createVertexBuffer(Indices, ARRAY_SIZE_IN_ELEMENTS(Indices));

		pEffect = new LightingTechnique();

		if (!pEffect->Init())
		{
			return false;
		}

		pEffect->Enable();

		pEffect->SetTextureUnit(0);

		pTexture = new Texture(GL_TEXTURE_2D, "C://test1.png");

		if (!pTexture->Load()) {
			return false;
		}

		return true; */
		if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
			return false;
		}

		pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

		pEffect = new LightingTechnique();

		if (!pEffect->Init()) {
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_pShadowMapTech = new ShadowMapTechnique();

		if (!m_pShadowMapTech->Init()) {
			printf("Error initializing the shadow map technique\n");
			return false;
		}

		m_pShadowMapTech->Enable();

		m_pQuad = new Mesh();

		if (!m_pQuad->LoadMesh("C://quad.obj")) {
			return false;
		}

		m_pMesh = new Mesh();

		return m_pMesh->LoadMesh("C://phoenix_ugv.md2"); 
	}

	void Run()
	{
		GLUTBackendRun(this);
	}

	virtual void RenderSceneCB()
	{
		pGameCamera->OnRender();
		sc += 0.05f;

		ShadowMapPass();
		RenderPass();

		glutSwapBuffers();


//		pGameCamera->OnRender();
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		sc += 0.3f;
//
///*		PointLight pl[3];
//		pl[0].DiffuseIntensity = 0.5f;
//		pl[0].Color = Vector3f(1.0f, 0.0f, 0.0f);
//		pl[0].Position = Vector3f(sinf(sc) * 10, 1.0f, cosf(sc) * 10);
//		pl[0].Attenuation.Linear = 0.1f;
//
//		pl[1].DiffuseIntensity = 0.5f;
//		pl[1].Color = Vector3f(0.0f, 1.0f, 0.0f);
//		pl[1].Position = Vector3f(sinf(sc + 2.1f) * 10, 1.0f, cosf(sc + 2.1f) * 10);
//		pl[1].Attenuation.Linear = 0.1f;
//
//		pl[2].DiffuseIntensity = 0.5f;
//		pl[2].Color = Vector3f(0.0f, 0.0f, 1.0f);
//		pl[2].Position = Vector3f(sinf(sc + 4.2f) * 10, 1.0f, cosf(sc + 4.2f) * 10);
//		pl[2].Attenuation.Linear = 0.1f;
//
//		pEffect->SetPointLights(3, pl); */
//
//		SpotLight sl[2];
//		sl[0].DiffuseIntensity = 15.0f;
//		sl[0].Color = Vector3f(1.0f, 1.0f, 0.7f);
//		sl[0].Position = Vector3f(-0.0f, -1.9f, -0.0f);
//		sl[0].Direction = Vector3f(sinf(sc), 0.0f, cosf(sc));
//		sl[0].Attenuation.Linear = 0.1f;
//		sl[0].Cutoff = 20.0f;
//
//		sl[1].DiffuseIntensity = 5.0f;
//		sl[1].Color = Vector3f(0.0f, 1.0f, 1.0f);
//		sl[1].Position = pGameCamera->GetPos();
//		sl[1].Direction = pGameCamera->GetTarget();
//		sl[1].Attenuation.Linear = 0.1f;
//		sl[1].Cutoff = 10.0f;
//
//		pEffect->SetSpotLights(2, sl);
//
//		// Создаем: едиинчную матрицу, матричу вращения, матрицу движения, матрицу размера и обьединяеим в итоговую матрицу result
//		/*glm::mat4x4 unit;
//		unit[0][0] = 1.0f; unit[0][1] = 0.0f; unit[0][2] = 0.0f; unit[0][3] = 0.0f;
//		unit[1][0] = 0.0f; unit[1][1] = 1.0f; unit[1][2] = 0.0f; unit[1][3] = 0.0f;
//		unit[2][0] = 0.0f; unit[2][1] = 0.0f; unit[2][2] = 1.0f; unit[2][3] = 0.0f;
//		unit[3][0] = 0.0f; unit[3][1] = 0.0f; unit[3][2] = 0.0f; unit[3][3] = 1.0f;
//
//		glm::mat4x4 rotate;
//		rotate[0][0] = sinf(sc); rotate[0][1] = -sinf(sc); rotate[0][2] = 0.0f; rotate[0][3] = 0.0f;
//		rotate[1][0] = 0.0f; rotate[1][1] = 1.0f; rotate[1][2] = 0.0f; rotate[1][3] = 0.0f;
//		rotate[2][0] = cosf(sc); rotate[2][1] = cosf(sc); rotate[2][2] = 1.0f; rotate[2][3] = 0.0f;
//		rotate[3][0] = 0.0f; rotate[3][1] = 0.0f; rotate[3][2] = 0.0f; rotate[3][3] = 1.0f;
//
//		glm::mat4x4 move;
//		move[0][0] = 1.0f; move[0][1] = 0.0f; move[0][2] = 0.0f; move[0][3] = sinf(sc);
//		move[1][0] = 0.0f; move[1][1] = 1.0f; move[1][2] = 0.0f; move[1][3] = cosf(sc);
//		move[2][0] = 0.0f; move[2][1] = 0.0f; move[2][2] = 1.0f; move[2][3] = 0.0f;
//		move[3][0] = 0.0f; move[3][1] = 0.0f; move[3][2] = 0.0f; move[3][3] = 1.0f;
//
//		glm::mat4x4 resize;
//		resize[0][0] = sinf(sc); resize[0][1] = 0.0f; resize[0][2] = 0.0f; resize[0][3] = 0.0f;
//		resize[1][0] = 0.0f; resize[1][1] = sinf(sc); resize[1][2] = 0.0f; resize[1][3] = 0.0f;
//		resize[2][0] = 0.0f; resize[2][1] = 0.0f; resize[2][2] = sinf(sc); resize[2][3] = 0.0f;
//		resize[3][0] = 0.0f; resize[3][1] = 0.0f; resize[3][2] = 0.0f; resize[3][3] = 1.0f;
//
//		glm::mat4x4 result = unit * rotate * move * resize;
//		glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, &result[0][0]); */
//
//		//создаем объект конвейера, настраиваем его и отправляем результат в шейдер.
//		Pipeline p;
//
//		p.Rotate(0.0f, sc, 0.0f);
//		p.WorldPos(0.0f, 0.0f, 3.0f);
//		p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
//		p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
//
//		pEffect->SetWVP(p.GetWVPTrans());
//
//		const Matrix4f& WorldTransformation = p.GetWorldTrans();
//		pEffect->SetWorldMatrix(WorldTransformation);
//
//		pEffect->SetDirectionalLight(directionalLight);
//
//		pEffect->SetEyeWorldPos(pGameCamera->GetPos());
//		pEffect->SetMatSpecularIntensity(1.0f);
//		pEffect->SetMatSpecularPower(32);
//
//		//	p.WorldPos(sinf(sc), 0.0f, 0.0f);
//		//	p.Rotate(sinf(sc) * 90.0f, sinf(sc) * 90.0f, sinf(sc) * 90.0f);
//		//	p.Scale(sinf(sc * 0.1f), sinf(sc * 0.1f), sinf(sc * 0.1f));
//		//	p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
//
//			//glm::vec3 CameraPos(0.0f, 0.0f, -3.0f);
//			//glm::vec3 CameraTarget(0.0f, 0.0f, 2.0f);
//			//glm::vec3 CameraUp(0.0f, 1.0f, 0.0f);
//			//p.SetCamera(CameraPos, CameraTarget, CameraUp);
//
//		//	p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
//
//		//используем атрибуты вершин
//		glEnableVertexAttribArray(0);
//		glEnableVertexAttribArray(1);
//		glEnableVertexAttribArray(2);
//		//привязка буфера для рисования
//		glBindBuffer(GL_ARRAY_BUFFER, VBO);
//		//устанавливаем атрибуты вершин
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
//		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
//		//привязка буфера для рисования
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
//
//		pTexture->Bind(GL_TEXTURE0);
//
//		//отрисовка
//		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
//		//отключаме атрбуты вершин
//		glDisableVertexAttribArray(0);
//		glDisableVertexAttribArray(1);
//		glDisableVertexAttribArray(2);
//		//	for (int i = 0; i < 10; i++)
//		//		glDrawArrays(GL_POLYGON, 0, i);
//			//меняем местами фоновый буфер и буфер рендера местами
//		glutSwapBuffers(); 
	}

	virtual void ShadowMapPass()
	{
		m_shadowMapFBO.BindForWriting();

		glClear(GL_DEPTH_BUFFER_BIT);

		Pipeline p;
		p.Scale(0.2f, 0.2f, 0.2f);
		p.Rotate(0.0f, sc, 0.0f);
		p.WorldPos(0.0f, 0.0f, 5.0f);
		p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
		p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f);
		m_pShadowMapTech->SetWVP(p.GetWVPTrans());
		m_pMesh->Render();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	virtual void RenderPass()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pShadowMapTech->SetTextureUnit(0);
		m_shadowMapFBO.BindForReading(GL_TEXTURE0);

		Pipeline p;
		p.Scale(5.0f, 5.0f, 5.0f);
		p.WorldPos(0.0f, 0.0f, 10.0f);
		p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
		p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f);
		m_pShadowMapTech->SetWVP(p.GetWVPTrans());
		m_pQuad->Render();
	}

	virtual void IdleCB()
	{
		RenderSceneCB();
	}

	virtual void SpecialKeyboardCB(int Key, int x, int y)
	{
		pGameCamera->OnKeyboard(Key);
	}

	virtual void KeyboardCB(unsigned char Key, int x, int y)
	{
		switch (Key) {
		case 'q':
			glutLeaveMainLoop();
			break;

	/*	case 'a':
			directionalLight.AmbientIntensity += 0.05f;
			break;

		case 's':
			directionalLight.AmbientIntensity -= 0.05f;
			break;

		case 'z':
			directionalLight.DiffuseIntensity += 0.05f;
			break;

		case 'x':
			directionalLight.DiffuseIntensity -= 0.05f;
			break; */
		}
	}

	virtual void PassiveMouseCB(int x, int y)
	{
		pGameCamera->OnMouse(x, y);
	}

private:

	/*void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount,
		Vertex* pVertices, unsigned int VertexCount) {
		for (unsigned int i = 0; i < IndexCount; i += 3) {
			unsigned int Index0 = pIndices[i];
			unsigned int Index1 = pIndices[i + 1];
			unsigned int Index2 = pIndices[i + 2];
			Vector3f v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
			Vector3f v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
			Vector3f Normal = v1.Cross(v2);
			Normal.Normalize();

			pVertices[Index0].m_normal += Normal;
			pVertices[Index1].m_normal += Normal;
			pVertices[Index2].m_normal += Normal;
		}

		for (unsigned int i = 0; i < VertexCount; i++) {
			pVertices[i].m_normal.Normalize();
		}
	}

	void createVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount)
	{
		Vertex Vertices[4] = { Vertex(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
                               Vertex(Vector3f(0.0f, -1.0f, -1.15475), Vector2f(0.5f, 0.0f)),
                               Vertex(Vector3f(1.0f, -1.0f, 0.5773f),  Vector2f(1.0f, 0.0f)),
                               Vertex(Vector3f(0.0f, 1.0f, 0.0f),      Vector2f(0.5f, 1.0f)) };

		unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(Vertices);

		CalcNormals(pIndices, IndexCount, Vertices, VertexCount);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	}

	void createIndexBuffer(const unsigned int* pIndices, unsigned int SizeInBytes)
	{
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeInBytes, pIndices, GL_STATIC_DRAW);
	}*/

//	GLuint VBO;
//	GLuint IBO;
	LightingTechnique* pEffect;
//	Texture* pTexture;
	Camera* pGameCamera;
	float sc;
//	DirectionalLight directionalLight;
	ShadowMapTechnique* m_pShadowMapTech;
	SpotLight m_spotLight;
	Mesh* m_pMesh;
	Mesh* m_pQuad;
	ShadowMapFBO m_shadowMapFBO;
};

int main(int argc, char** argv)
{
	GLUTBackendInit(argc, argv);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "OpenGL tutors")) {
		return 1;
	}

	Main* pApp = new Main();

	if (!pApp->Init()) {
		return 1;
	}

	pApp->Run();

	delete pApp;

	return 0;
}

