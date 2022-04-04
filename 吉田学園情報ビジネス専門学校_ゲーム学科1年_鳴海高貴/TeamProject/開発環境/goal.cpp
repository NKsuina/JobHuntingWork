//-----------------------------------------------------------------------------------------------------------
//
//�S�[��	
//Author; Takano Minto
//
//-----------------------------------------------------------------------------------------------------------
#include<stdio.h>
#include"goal.h"
#include"camera.h"

//--------------------------
//�}�N����`
//--------------------------
#define MAX_MODEL				(128)							//�z�u�o���郂�f���̍ő吔
#define SCAN_SPELL				(1200)							//�ǂݍ��݂ł���ő啶����
#define LOAD_FALE				"data/Text/goal.txt"			//�ǂݍ��ރe�L�X�g�t�@�C��

//--------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------
static GOAL s_aGoal[MAX_MODEL];							//�S�[���̍\���̏��
static int s_nModelCount;								//���f���p�̃J�E���g
static int s_nPartsCount;								//���f�����iPOS�Ȃǁj�p�̃J�E���g

//----------------------
//������
//----------------------
void InitGoal(void)
{
	//�e���f���J�E���g�ϐ��̏�����
	s_nModelCount = 0;
	s_nPartsCount = 0;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̃|�C���^

	// �t�@�C���|�C���^�̐錾
	FILE * pFile;

	pFile = fopen(LOAD_FALE, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		char aMoji[SCAN_SPELL];

		fscanf(pFile, "%s", &aMoji[0]);		//�J�����t�@�C������ǂݍ���

		while (strncmp(&aMoji[0], "SCRIPT", 6) != 0)
		{
			fscanf(pFile, "%s", &aMoji[0]);
		}

		while (strncmp(&aMoji[0], "END_SCRIPT", 10) != 0)
		{// ������̏������Ɠǂݍ���
			fscanf(pFile, "%s", &aMoji[0]);
			
			if (strcmp(&aMoji[0], "MODEL_FILENAME") == 0)
			{// �����񂪈�v�����ꍇ
				fscanf(pFile, "%s", &aMoji[0]);		//�h��"��ǂݔ�΂�
				fscanf(pFile, "%s", &aMoji[0]);		//��������ǂݍ���

				//X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX(&aMoji[0],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&s_aGoal[s_nModelCount].pBuffMat,
					NULL,
					&s_aGoal[s_nModelCount].nNumMat,
					&s_aGoal[s_nModelCount].pMesh);
		
				s_nModelCount++;	
			}
			else if (strcmp(&aMoji[0], "MODELSET") == 0)
			{// �����񂪈�v�����ꍇ
				
				while (strcmp(&aMoji[0], "END_MODELSET") != 0)
				{
					fscanf(pFile, "%s", &aMoji[0]);

					if (strcmp(&aMoji[0], "POS") == 0)
					{// �����񂪈�v�����ꍇ
						fscanf(pFile, "%s", &aMoji[0]);
						fscanf(pFile, "%f", &s_aGoal[s_nPartsCount].pos.x);
						fscanf(pFile, "%f", &s_aGoal[s_nPartsCount].pos.y);
						fscanf(pFile, "%f", &s_aGoal[s_nPartsCount].pos.z);
					}

					if (strcmp(&aMoji[0], "ROT") == 0)
					{// �����񂪈�v�����ꍇ
						fscanf(pFile, "%s", &aMoji[0]);
						fscanf(pFile, "%f", &s_aGoal[s_nPartsCount].rot.x);
						fscanf(pFile, "%f", &s_aGoal[s_nPartsCount].rot.y);
						fscanf(pFile, "%f", &s_aGoal[s_nPartsCount].rot.z);
					}

					if (strcmp(&aMoji[0], "COLLITION") == 0)
					{// �����񂪈�v�����ꍇ
						fscanf(pFile, "%s", &aMoji[0]);
						fscanf(pFile, "%f", &s_aGoal[s_nPartsCount].size.x);
						fscanf(pFile, "%f", &s_aGoal[s_nPartsCount].size.y);
						fscanf(pFile, "%f", &s_aGoal[s_nPartsCount].size.z);
					}
				}

				s_nPartsCount++;
			}
			else if (strcmp(&aMoji[0], "END_SCRIPT") == 0)
			{// �����񂪈�v�����ꍇ
				break;
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ��ꍇ
		printf("\n * * * �t�@�C����...�J���܂���ł����b�b�b!!!!!!!!! * * * \n");
	}

	int nNumVtx;				//���_�̎�� (��)
	DWORD SizeFVF;				//���_�Ɏg�p����t�H�[�}�b�g�i�����j
	BYTE *pVtxBuff;				//�ꎞ�I�ɏ����������߂Ă������_�̃|�C���^
	
	for (int nUse = 0; nUse < MAX_MODEL; nUse++)
	{//MIN�EMAX���̏�����
		s_aGoal[nUse].Min = D3DXVECTOR3(5000.0f, 5000.0f, 5000.0f);
		s_aGoal[nUse].Max = D3DXVECTOR3(-5000.0f, -5000.0f, -5000.0f);
		s_aGoal[nUse].bUse = false;
	}

	//X�t�@�C���Ăэ��ݎ��ɃJ�E���g����s_nModelCount����for����
	for (int nCnt = 0; nCnt < s_nModelCount; nCnt++)
	{
		SetGoal(s_aGoal[nCnt].pos, s_aGoal[nCnt].rot);
	}

	//X�t�@�C���Ăэ��ݎ��ɃJ�E���g����s_nModelCount����for����
	for (int nCnt = 0; nCnt < s_nModelCount; nCnt++)
	{
		//���_���̎擾����
		nNumVtx = s_aGoal[nCnt].pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		SizeFVF = D3DXGetFVFVertexSize(s_aGoal[nCnt].pMesh->GetFVF());

		//���_�������b�N����
		s_aGoal[nCnt].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCollition = 0; nCollition < nNumVtx; nCollition++)
		{
			//���_���̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//���_���W�̔�r
			if (vtx.x < s_aGoal[nCnt].Min.x)
			{
				s_aGoal[nCnt].Min.x = vtx.x;	//�l������Ɠ����ɂȂ�悤�ɐݒ肷��
			}

			if (vtx.x > s_aGoal[nCnt].Max.x)
			{
				s_aGoal[nCnt].Max.x = vtx.x;	//�l������Ɠ����ɂȂ�悤�ɐݒ肷��
			}

			if (vtx.z < s_aGoal[nCnt].Min.z)
			{
				s_aGoal[nCnt].Min.z = vtx.z;	//�l������Ɠ����ɂȂ�悤�ɐݒ肷��
			}

			if (vtx.z > s_aGoal[nCnt].Max.z)
			{
				s_aGoal[nCnt].Max.z = vtx.z;	//�l������Ɠ����ɂȂ�悤�ɐݒ肷��
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���̃|�C���^�����߂�(�H)
			pVtxBuff += SizeFVF;
		}

		//���_�o�b�t�@���A�����b�N����
		s_aGoal[nCnt].pMesh->UnlockVertexBuffer();

		// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɕϊ����Ď擾
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_aGoal[nCnt].pBuffMat->GetBufferPointer();

		//�e���b�V���̃}�e���A�������擾����
		for (int nCntTex = 0; nCntTex < (int)s_aGoal[nCnt].nNumMat; nCntTex++)
		{//�����ϊ����ꂽ�}�e���A���̎�ޕ�for����
			s_aGoal[nCnt].pTexture[nCntTex] = NULL;

			if (pMat[nCntTex].pTextureFilename != NULL)
			{//�}�e���A���Őݒ肳��Ă���e�N�X�`�����ǂݍ��܂�Ă���
				D3DXCreateTextureFromFileA(pDevice,
					pMat[nCntTex].pTextureFilename,
					&s_aGoal[nCnt].pTexture[nCntTex]);
			}
			else
			{//�}�e���A���Őݒ肳��Ă���e�N�X�`�����ǂݍ��܂�Ă��Ȃ�
				s_aGoal[nCnt].pTexture[nCntTex] = NULL;
			}
		}

	}

}

//----------------------
//�I������
//----------------------
void UninitGoal(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		for (int nCnt = 0; nCnt < (int)s_aGoal[nCntModel].nNumMat; nCnt++)
		{//�����ϊ����ꂽ�}�e���A���̎�ޕ�for����
			if (s_aGoal[nCntModel].pTexture[nCnt] != NULL)
			{// �e�N�X�`���̉��
				s_aGoal[nCntModel].pTexture[nCnt]->Release();
				s_aGoal[nCntModel].pTexture[nCnt] = NULL;
			}
		}

		//���b�V���̔j��
		if (s_aGoal[nCntModel].pMesh != NULL)
		{
			s_aGoal[nCntModel].pMesh->Release();
			s_aGoal[nCntModel].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (s_aGoal[nCntModel].pBuffMat != NULL)
		{
			s_aGoal[nCntModel].pBuffMat->Release();
			s_aGoal[nCntModel].pBuffMat = NULL;
		}

	}
}

//----------------
//�X�V
//----------------
void UpdateGoal(void)
{

}

//----------------
//�`��
//----------------
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̃|�C���^

	D3DXMATRIX mtxRot, mtxTrans;					//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;							//���݂̃}�g���b�N�X�ۑ��p
	D3DXMATERIAL *pMat;								//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCnt = 0; nCnt < s_nModelCount; nCnt++)
	{
		if (s_aGoal[nCnt].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&s_aGoal[nCnt].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, s_aGoal[nCnt].rot.y, s_aGoal[nCnt].rot.x, s_aGoal[nCnt].rot.z);

			D3DXMatrixMultiply(&s_aGoal[nCnt].mtxWorld, &s_aGoal[nCnt].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, s_aGoal[nCnt].pos.x, s_aGoal[nCnt].pos.y, s_aGoal[nCnt].pos.z);

			D3DXMatrixMultiply(&s_aGoal[nCnt].mtxWorld, &s_aGoal[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�ɐݒ�
			pDevice->SetTransform(D3DTS_WORLD, &s_aGoal[nCnt].mtxWorld);

			//���݂̃}�e���A���ێ�
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)s_aGoal[nCnt].pBuffMat->GetBufferPointer();

			for (int nCntModel = 0; nCntModel < (int)s_aGoal[nCnt].nNumMat; nCntModel++)
			{//�����ϊ����ꂽ�}�e���A���̎�ޕ�for����
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntModel].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, s_aGoal[nCnt].pTexture[nCntModel]);

				//���f���̕`��
				s_aGoal[nCnt].pMesh->DrawSubset(nCntModel);
			}
		}
	}

	//�ێ����Ă����}�e���A����ԋp
	pDevice->SetMaterial(&matDef);

}

//---------------------------
//�S�[���̐ݒu
//---------------------------
void SetGoal(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		if (s_aGoal[nCnt].bUse == false)
		{//�A�C�e�����g�p����Ă��Ȃ���
			s_aGoal[nCnt].bUse = true;
			int nNumVtx;
			DWORD SizeFVF;
			BYTE *pVtxBuff;
			s_aGoal[nCnt].Min = D3DXVECTOR3(5000.0f, 5000.0f, 5000.0f);
			s_aGoal[nCnt].Max = D3DXVECTOR3(-5000.0f, -5000.0f, -5000.0f);
			s_aGoal[nCnt].pos = pos;
			s_aGoal[nCnt].rot = rot;

			//���_���̎擾����
			nNumVtx = s_aGoal[0].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			SizeFVF = D3DXGetFVFVertexSize(s_aGoal[0].pMesh->GetFVF());

			s_aGoal[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCollition = 0; nCollition < s_nModelCount; nCollition++)
			{
				//���_���̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//���_���W�̔�r
				if (vtx.x < s_aGoal[nCollition].Min.x)
				{
					s_aGoal[0].Min.x = vtx.x;	//�l������Ɠ����ɂȂ�悤�ɐݒ肷��
				}

				if (vtx.x > s_aGoal[nCollition].Max.x)
				{
					s_aGoal[nCollition].Max.x = vtx.x;	//�l������Ɠ����ɂȂ�悤�ɐݒ肷��
				}

				if (vtx.z < s_aGoal[nCollition].Min.z)
				{
					s_aGoal[nCollition].Min.z = vtx.z;	//�l������Ɠ����ɂȂ�悤�ɐݒ肷��
				}

				if (vtx.z > s_aGoal[nCollition].Max.z)
				{
					s_aGoal[nCollition].Max.z = vtx.z;	//�l������Ɠ����ɂȂ�悤�ɐݒ肷��
				}

				//���_�t�H�[�}�b�g�̃T�C�Y���̃|�C���^�����߂�(�H)
				pVtxBuff += SizeFVF;
			}

			//���_�o�b�t�@���A�����b�N����
			s_aGoal[0].pMesh->UnlockVertexBuffer();

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	s_aGoal[0].pMesh->UnlockVertexBuffer();
	
}

//--------------------------
//�����蔻��
//--------------------------
void CollitionGoal(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 size)
{
	for (int i = 0; i < MAX_MODEL; i++)
	{
		if (s_aGoal[i].bUse == true)
		{
			float fRight = s_aGoal[i].pos.x + s_aGoal[i].Min.x;
			float fLeft = s_aGoal[i].pos.x + s_aGoal[i].Max.x;
			float fBack = s_aGoal[i].pos.z + s_aGoal[i].Max.z;
			float fFlont = s_aGoal[i].pos.z + s_aGoal[i].Min.z;

			size = s_aGoal[i].size;

			if (pPos->x + size.x > fRight &&
				pPos->x - size.x < fLeft)
			{
				if (pPosOld->z + size.z <= fFlont &&
					pPos->z + size.z > fFlont)
				{//�O������̔���
					pPos->z = fFlont - size.z - 0.0001f;
				}

				if (pPosOld->z - size.z >= fBack &&
					pPos->z - size.z < fBack)
				{
					pPos->z = fBack + size.z + 0.0001f;
				}
			}

			if (pPos->z + size.z > fFlont &&
				pPos->z - size.z < fBack)
			{
				if (pPosOld->x + size.x <= fRight &&
					pPos->x + size.x > fRight)
				{//�O������̔���
					pPos->x = fRight - size.x - 0.0001f;
				}

				if (pPosOld->x - size.x >= fLeft &&
					pPos->x - size.x < fLeft)
				{
					pPos->x = fLeft + size.x + 0.0001f;
				}
			}
		}
	}
}

//--------------------------
//�S�[�����̎擾
//--------------------------
GOAL *GetGoal(void)
{
	return s_aGoal;
}