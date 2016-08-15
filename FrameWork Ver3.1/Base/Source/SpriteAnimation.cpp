#include "SpriteAnimation.h"
#include "GL\glew.h"
#include "Vertex.h"

SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col) :Mesh(meshName)
{
	this->m_row = row;
	this->m_col = col;
	this->m_currentTime = 0.f;
	this->m_currentFrame = 0;
	this->m_playCount = 0;
	m_anim = nullptr;
}

SpriteAnimation::~SpriteAnimation()
{
	if (m_anim)
	{
		delete m_anim;
	}
}

void SpriteAnimation::Update(double dt)
{
	if (m_anim)
	{
		if (m_anim->animActive == true)
		{
			// ANIMATION + DT
			m_currentTime += (float)dt;

			// FIND INT NUMFRAME
			int numFrame = Math::Max(1, m_anim->endFrame - m_anim->startFrame + 1);

			// GET FLOAT FRAMETIME
			float frameTime = m_anim->animTime / numFrame;

			//GET ANIMATION CURRENT FRAME
			m_currentFrame = Math::Min(m_anim->startFrame + (int)(m_currentTime / frameTime), m_anim->endFrame);

			// CHECK IF M_CURRENTTIME IS >= ANIMTIME
			if (m_currentTime >= m_anim->animTime)
			{
				if (m_anim->repeatCount == 0)
				{
					m_anim->animActive = false;
					m_currentTime = 0.f;
					m_currentFrame = m_anim->startFrame;
				}
				else if (m_anim->repeatCount == 1)
				{
					m_currentTime = 0;
					m_currentFrame = m_anim->startFrame;
				}
			}
		}
	}

}

void SpriteAnimation::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position)+sizeof(Color)));
	//if(textureID > 0)
	{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position)+sizeof(Color)+sizeof(Vector3)));
	}


	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if (mode == DRAW_LINES)
		glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));
	else if (mode == DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT,(void*)(m_currentFrame * 6 * sizeof(GLuint)) );
	else
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//if(textureID > 0)
	{
		glDisableVertexAttribArray(3);
	}
}