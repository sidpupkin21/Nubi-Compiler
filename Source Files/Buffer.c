/*
**********************************************************************
* Code version: 1.0
* Starter Files Author: Svillen Ranev & Paulo Sousa
* Author: Ahmed Mohamed & Jeel Patel
* Last Edited: Feb 5, 2022
**********************************************************************
*/

/*
**********************************************************************
* File name: buffer.c
* Compiler: MS Visual Studio 2022
* Purpose: This file is the main code for Buffer
* Function List: bCreate(), bAddChar(), bClear(), bDestroy(), 
*		bIsFull(), bGetWritePos(), bGetSize(), bGetMode(), 
*		bGetMarkPos(), bSetMark(), bPrint(), bLoad(), bIsEmpty(),
*		bGetChar(), bRecover(), bRetract(), bRestore(), bGetReadPos(),
*		bGetIncrement(), bGetContent(), bGetFlags()
**********************************************************************
*/

#ifndef COMPILERS_H_
#include "Header Files/Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Header Files/Buffer.h"
#endif

/*
***********************************************************
* Function name: bCreate
* Purpose: Creates the buffer according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: Ver 2.1
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to Buffer)
* Algorithm: Allocation of memory according to inicial (default) values.
* 
*************************************************************
*/
BufferPointer bCreate(Sint size, Sint increment, Sint mode) {
	BufferPointer b;
	if (size < 0) {
		return NULL;
	}
	else if (size > NUBI_MAX_SIZE) {
		return NULL;
	}
	if (!increment) {
		mode = MODE_FIXED;
	}
	b = (BufferPointer)calloc(1, sizeof(Buffer));
	while (b == NULL) {
		return NULL;
	}
	b->string = (chars*)malloc(size);
	if (!b->string) {
		return NULL;
	}
	return b;

}


/*
***********************************************************
* Function name: bAddChar
* Purpose: Adds a char to buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   ch = char to be added
* Return value:
*	bPointer (pointer to Buffer)
*************************************************************
*/

BufferPointer bAddChar(BufferPointer const pBuffer, chars ch) {
	Sint newSize = 0;
	if (pBuffer == NULL) {
		return NULL;
	}
	switch (pBuffer->mode) {
	case MODE_FIXED:
		return NULL;
	case MODE_ADDIT:
		newSize = pBuffer->size + pBuffer->increment;
		if (newSize < pBuffer->size) {
			return NULL;
		}
		else if (newSize < 0) {
			return NULL;
		}
	case MODE_MULTI:
		newSize = pBuffer->size * pBuffer->increment;
		if (newSize < pBuffer->size) {
			return NULL;
		}
		else if (newSize < 0) {
			return NULL;
		}
	}
	pBuffer->string[pBuffer->position.writePos++] = ch;
	return pBuffer;
}

/*
***********************************************************
* Function name: bClear
* Purpose: Clears the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
*************************************************************
*/
Usign bClear(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return NUBI_F;
	}
	pBuffer->position.writePos = pBuffer->position.markPos = pBuffer->position.readPos = 0;
	pBuffer->flags = RST_EOB_FLAG;
	return NUBI_T;
}

/*
***********************************************************
* Function name: bDestroy
* Purpose: Releases the buffer address
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
*************************************************************
*/
Usign bDestroy(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return NUBI_F;
	}
	//free(pBuffer);
	return NUBI_T;
}

/*
***********************************************************
* Function name: bIsFull
* Purpose: Checks if buffer is full
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
*************************************************************
*/
Usign bIsFull(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return NUBI_F;
	}
	if (pBuffer->size == pBuffer->position.writePos * (Sint)sizeof(chars)) {
		return NUBI_T;
	}
	return NUBI_T;
}

/*
***********************************************************
* Function name: bGetWritePos
* Purpose: Returns the position of char to be added
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	addcPosition value
*************************************************************
*/
Sint bGetWritePos(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return BUFFER_ERROR;
	}
	return pBuffer->position.writePos;
}

/*
***********************************************************
* Function name: bGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Size of buffer.
*************************************************************
*/
Sint bGetSize(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return BUFFER_ERROR;
	}
	return pBuffer->size;
}

/*
***********************************************************
* Function name: bGetMode
* Purpose: Returns the operational mode
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	operational mode.
*************************************************************
*/
Sint bGetMode(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return (chars)BUFFER_ERROR;
	}
	return pBuffer->mode;
}


/*
***********************************************************
* Function name: bGetMarkPos
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	mark offset.
*************************************************************
*/
Sint bGetMarkPos(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return BUFFER_ERROR;
	}
	return pBuffer->position.markPos;
}


/*
***********************************************************
* Function name: bSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
*************************************************************
*/
Usign bSetMark(BufferPointer const pBuffer, Sint mark) {
	if (pBuffer == NULL) {
		return BUFFER_ERROR;
	}
	pBuffer->position.markPos = mark;
	return NUBI_T;
}

/*
***********************************************************
* Function name: bPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Number of chars printed.
*************************************************************
*/
Sint bPrint(BufferPointer const pBuffer) {
	Sint size = 1;
	chars c;
	if (pBuffer == NULL) {
		return BUFFER_ERROR;
	}
	c = bGetChar(pBuffer);
	while (c > 0) {
		printf("%c", c);
		c = bGetChar(pBuffer);
		size++;
	}
	return size;
}

/*
***********************************************************
* Function name: bLoad
* Purpose: Loads the string in the buffer with the content of
*	an specific file.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   fi = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
*************************************************************
*/
Sint bLoad(BufferPointer const pBuffer, FILE* const fi) {
	Sint size = 1;
	chars c;
	if (pBuffer == NULL) {
		return BUFFER_ERROR;
	}
	c = (chars)fgetc(fi);
	while (!feof(fi)) {
		if (!bAddChar(pBuffer, c)) {
			ungetc(c, fi);
			return BUFFER_ERROR;
		}
		c = (char)fgetc(fi);
	}
	return size;
}

/*
***********************************************************
* Function name: bIsEmpty
* Purpose: Checks if buffer is empty.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
*************************************************************
*/
Usign bIsEmpty(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return NUBI_F;
	}
	if (pBuffer->position.writePos == 0) {
		return NUBI_T;
	}
	return NUBI_F;
}

/*
***********************************************************
* Function name: bGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Char in the getC position.
*************************************************************
*/
chars bGetChar(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return BUFFER_ERROR;
	}
	if (pBuffer->position.writePos == pBuffer->position.readPos) {
		return SET_EOB_FLAG;
	}
	return pBuffer->string[pBuffer->position.readPos++];
}


/*
***********************************************************
* Function name: bRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value
*	Boolean value about operation success
*************************************************************
*/
Usign bRecover(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return BUFFER_ERROR;
	}
	pBuffer->position.readPos = 0;
	pBuffer->position.markPos = 0;
	return NUBI_T;
}


/*
***********************************************************
* Function name: bRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
*************************************************************
*/
Usign bRetract(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return NUBI_F;
	}
	pBuffer->position.readPos--;
	return NUBI_T;
}


/*
***********************************************************
* Function name: bRestore
* Purpose: Resets the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
*************************************************************
*/
Usign bRestore(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return NUBI_F;
	}
	pBuffer->position.readPos = pBuffer->position.markPos;
	return NUBI_T;
}


/*
***********************************************************
* Function name: bGetReadPos
* Purpose: Returns the value of getCPosition.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The readPos offset.
*************************************************************
*/
Sint bGetReadPos(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return BUFFER_ERROR;
	}
	/*if (pBuffer->string == NULL) {
		return BUFFER_ERROR;
	}*/
	return pBuffer->position.readPos;
}


/*
***********************************************************
* Function name: bGetIncrement
* Purpose: Returns the buffer increment.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The Buffer increment.
*************************************************************
*/
Sint bGetIncrement(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		return BUFFER_ERROR;
	}
	return pBuffer->increment;
}


/*
***********************************************************
* Function name: bGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   pos = position to get the pointer
* Return value:
*	Position of string char.
*************************************************************
*/
chars* bGetContent(BufferPointer const pBuffer, Sint pos) {
	if (pos < 0 || pBuffer == NULL) {
		return NULL;
	}
	if (pos > pBuffer->position.writePos) {
		return  NULL;
	}
	return pBuffer->string + pos;
}


/*
***********************************************************
* Function name: bGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Flags from Buffer.
*************************************************************
*/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
Sign bGetFlags(BufferPointer const pBuffer) {
	if (pBuffer == NULL) {
		//return NUBI_DEFAULT_FLAG;
		return BUFFER_ERROR;
	}
	return pBuffer->flags;
}
#else
#define bGetFlags(pBuffer) ((pBuffer)?(pBuffer->flags):(RT_FAIL_1))
#endif

