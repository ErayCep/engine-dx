#include "font.h"

Font::Font() {
	m_font = 0;
	m_texture = 0;
}

Font::Font(const Font&) {}

Font::~Font() {}

bool Font::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int fontChoice) {
	char fontFilename[128];
	char fontTextureFilename[128];
	bool result;

	switch (fontChoice)
	{
	case 0:
		strcpy_s(fontFilename, "C:\\Users\\cepni\\source\\repos\\dx11\\assets\\fonts\\font01.txt");
		strcpy_s(fontTextureFilename, "C:\\Users\\cepni\\source\\repos\\dx11\\assets\\fonts\\font01.tga");
		m_fontHeight = 32.0f;
		m_spaceSize = 3;
		break;
	default:
		strcpy_s(fontFilename, "C:\\Users\\cepni\\source\\repos\\dx11\\assets\\fonts\\font01.txt");
		strcpy_s(fontTextureFilename, "C:\\Users\\cepni\\source\\repos\\dx11\\assets\\fonts\\font01.tga");
		m_fontHeight = 32.0f;
		m_spaceSize = 3;
		break;
	}

	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	result = LoadTexture(device, deviceContext, fontTextureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Font::Shutdown() {
	ReleaseTexture();

	ReleaseFontData();

	return;
}

bool Font::LoadFontData(char* filename) {
	ifstream fin;
	int i;
	char temp;

	m_font = new FontType[95];

	fin.open(filename);
	if (fin.fail()) {
		return false;
	}

	for (i = 0; i < 95; i++) {
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_font[i].left;
		fin >> m_font[i].right;
		fin >> m_font[i].size;
	}

	fin.close();

	return true;
}

void Font::ReleaseFontData() {
	if (m_font) {
		delete[] m_font;
		m_font = 0;
	}

	return;
}

bool Font::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename) {
	bool result;


	// Create and initialize the font texture object.
	m_texture = new TextureClass;

	result = m_texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

}

void Font::ReleaseTexture() {
	if (m_texture) {
		delete m_texture;
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* Font::GetTexture() {
	return m_texture->GetTexture();
}

void Font::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY) {
	VertexType* vertexPtr;
	int numLetters, index, i, letter;

	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;

	for (i = 0; i < numLetters; i++) {
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if (letter == 0)
		{
			drawX = drawX + m_spaceSize;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_font[letter].size), (drawY - m_fontHeight), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - m_fontHeight), 0.0f);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_font[letter].size), (drawY - m_fontHeight), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_font[letter].size + 1.0f;
		}
	}

	return;
}

int Font::GetSentencePixelLength(char* sentence) {
	int pixelLength, numLetters, i, letter;

	pixelLength = 0;
	numLetters = (int)strlen(sentence);

	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then count it as three pixels.
		if (letter == 0)
		{
			pixelLength += m_spaceSize;
		}
		else
		{
			pixelLength += (m_font[letter].size + 1);
		}
	}

	return pixelLength;
}

int Font::GetFontHeight()
{
	return (int)m_fontHeight;
}