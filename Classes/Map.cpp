#include "Map.h"

void Terrain::updateView()
{
	if (nowStartGrid != oldStartGrid)
	{
		oldCells = nowCells;
		for (int nowIndex = 0; nowIndex < nowCells.size(); nowIndex++)
		{
			bool not = true;
			Point gloabIndex = getGloabIndex(nowStartGrid, nowIndex); 	// ���ݵ�ǰƫ�� �µı�� ��ȡȫ��λ��
			int oldIndex = getLocalIndex(oldStartGrid, gloabIndex); 		// �����ϵ�ƫ�� ȫ��λ�� ��ȡ�ϵı��
			if (oldIndex < oldCells.size() && oldIndex >= 0 && oldCells[oldIndex])
			{
				nowCells[nowIndex] = oldCells[oldIndex];
				if (gloabIndex == nowCells[nowIndex]->gloabIndex)
				{
					//	log("gloab index cant delete oldStartG:<%f,%f> startG:<%f,%f> oldG<%f,%f> nowG:<%f,%f>index:%d oldIndex:%d",oldStartGrid.x,oldStartGrid.y,nowStartGrid.x,nowStartGrid.y,nowCells[nowIndex]->gloabIndex.x,nowCells[nowIndex]->gloabIndex.y,gloabIndex.x,gloabIndex.y,nowIndex,oldIndex);
					nowCells[nowIndex]->nowIndex = nowIndex;
					oldCells[oldIndex] = NULL;
					not = false;
				}
				else
				{
					//	log("gloab index error oldStartG:<%f,%f> startG:<%f,%f> oldG<%f,%f> nowG:<%f,%f>index:%d oldIndex:%d",oldStartGrid.x,oldStartGrid.y,nowStartGrid.x,nowStartGrid.y,nowCells[nowIndex]->gloabIndex.x,nowCells[nowIndex]->gloabIndex.y,gloabIndex.x,gloabIndex.y,nowIndex,oldIndex);
				}
			}
			if (not)
			{
				nowCells[nowIndex] = new MapCell();

				this->addChild(nowCells[nowIndex]);

				// ������Դ
				nowCells[nowIndex]->gloabIndex = gloabIndex;
				nowCells[nowIndex]->nowIndex = nowIndex;
				Size gridScale;
				gridScale.width = 8;
				gridScale.height = 8;
				nowCells[nowIndex]->setUrl(url.c_str());
				nowCells[nowIndex]->updateView(mapName.c_str(), gridScale); // ������ͼ 

				nowCells[nowIndex]->release();
			}
			if (nowCells[nowIndex])
			{
				// ����Cell��λ��
				nowCells[nowIndex]->updatePoistion(gridPixelSize);
			}
		}
		for (int oldIndex = 0; oldIndex < oldCells.size(); oldIndex++)
		{
			if (oldCells[oldIndex])
			{
				this->removeChild(oldCells[oldIndex]);
			}
		}
		oldCells.clear();
		oldStartGrid = nowStartGrid;
	}
}