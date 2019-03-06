// Includes
//=========

#include "cTexture.h"

#include <cstring>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <new>

// Static Data Initialization
//===========================

eae6320::Assets::cManager<eae6320::Graphics::cTexture> eae6320::Graphics::cTexture::s_manager;

// Interface
//==========

// Access
//-------

uint16_t eae6320::Graphics::cTexture::GetWidth() const
{
	return m_info.width;
}

uint16_t eae6320::Graphics::cTexture::GetHeight() const
{
	return m_info.height;
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::Graphics::cTexture::Load( const std::string& i_path, cTexture*& o_texture )
{
	auto result = Results::Success;

	Platform::sDataFromFile dataFromFile;
	cTexture* newTexture = nullptr;
	uintptr_t currentOffset;
	uintptr_t finalOffset;

	// Load the binary data
	{
		std::string errorMessage;
		if ( !( result = Platform::LoadBinaryFile( i_path.c_str(), dataFromFile, &errorMessage ) ) )
		{
			EAE6320_ASSERTF( false, errorMessage.c_str() );
			Logging::OutputError( "Failed to load texture data from file %s: %s", i_path.c_str(), errorMessage.c_str() );
			goto OnExit;
		}
	}

	// Extract data from the file
	currentOffset = reinterpret_cast<uintptr_t>( dataFromFile.data );
	finalOffset = currentOffset + dataFromFile.size;

	// The file starts with information about the texture
	{
		const auto size = sizeof( TextureFormats::sTextureInfo );
		const auto newOffset = currentOffset + size;
		if ( newOffset <= finalOffset )
		{
			const auto* const textureInfo = reinterpret_cast<TextureFormats::sTextureInfo*>( currentOffset );
			EAE6320_ASSERT( ( textureInfo->width % 4u ) == 0u ); 
			EAE6320_ASSERT( ( textureInfo->height % 4u ) == 0u );
			currentOffset = newOffset;
			// Allocate a new texture with the information
			newTexture = new (std::nothrow) cTexture( *textureInfo );
			if ( !newTexture )
			{
				result = Results::OutOfMemory;
				EAE6320_ASSERTF( false, "Couldn't allocate memory for the texture %s", i_path.c_str() );
				Logging::OutputError( "Failed to allocate memory for the texture %s", i_path.c_str() );
				goto OnExit;
			}
		}
		else
		{
			result = Results::InvalidFile;
			EAE6320_ASSERTF( false, "The texture file %s is too small (%u) to include texture information (%u)",
				i_path.c_str(), dataFromFile.size, size );
			Logging::OutputError( "The texture file %s is too small (%u) to include texture information (%u)",
				i_path.c_str(), dataFromFile.size, size );
			goto OnExit;
		}
	}
	// The rest of the data is pixel information that is used to create platform-specific textures
	{
		const auto* const textureData = reinterpret_cast<void*>( currentOffset );
		const auto textureDataSize = static_cast<size_t>( finalOffset - currentOffset );
		if ( !( result = newTexture->Initialize( i_path, textureData, textureDataSize ) ) )
		{
			EAE6320_ASSERTF( false, "Initialization of new texture failed" );
			goto OnExit;
		}
	}

OnExit:

	if ( result )
	{
		EAE6320_ASSERT( newTexture );
		o_texture = newTexture;
	}
	else
	{
		if ( newTexture )
		{
			newTexture->DecrementReferenceCount();
			newTexture = nullptr;
		}
		o_texture = nullptr;
	}
	dataFromFile.Free();

	return result;
}

// Implementation
//===============

// Initialization / Clean Up
//--------------------------

eae6320::Graphics::cTexture::cTexture( const TextureFormats::sTextureInfo i_info )
	:
	m_info( i_info )
{

}

eae6320::Graphics::cTexture::~cTexture()
{
	EAE6320_ASSERT( m_referenceCount == 0 );
	const auto result = CleanUp();
	EAE6320_ASSERT( result );
}
