#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

struct FileSign {
	LPVOID MZHeader;
	DWORD NTHeader;
	WORD Machine;
	WORD Magic;
};


LPVOID _OpenFile(const char *str);
size_t _ReadData(LPVOID FileBuffer, struct FileSign *FileSign);


int main() {
	char str[100];
	printf("输入程序路径\n");
	gets(str);
	struct FileSign* pFileSign , filesign;
	pFileSign = &filesign;

	LPVOID FileBuffer = _OpenFile(str);
	if (!FileBuffer) {
		system("pause");
		return 0;
	}
	if (!_ReadData(FileBuffer, pFileSign)) {
		printf("不是可执行文件\n");
		return 0;
	}

	
	//判断是64位还是32位
	if (pFileSign->Magic == 0x10B) {
		printf ("32位程序\n");
	}
	if (pFileSign->Magic == 0x20B) {
		printf("64位程序\n");
	}
	system("pause");

}

LPVOID _OpenFile(const char *str) {
	FILE *pf = fopen(str, "rb");
	if (!pf) {
		perror("打开文件失败");
		return NULL;
	}
	fseek(pf, 0, SEEK_END);
	DWORD FileSize = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	LPVOID FileBuffer = (char*)malloc(FileSize);
	fread(FileBuffer, 1, FileSize, pf);
	fclose(pf);
	return FileBuffer;

}

size_t _ReadData(LPVOID FileBuffer, struct FileSign *FileSign) {
	FileSign->MZHeader = *(WORD*)((char*)FileBuffer);
	if (FileSign->MZHeader != 0x5a4d) {
		return 0;
	}
	FileSign->NTHeader = (char*)((char*)FileBuffer+(*(DWORD *)((char*)FileBuffer + 0x3C)));
	FileSign->Machine = *(WORD*)((char*)FileSign->NTHeader + 0x4);
	FileSign->Magic = *(WORD*)((char*)FileSign->NTHeader + 0x18);
	return 1;
}

