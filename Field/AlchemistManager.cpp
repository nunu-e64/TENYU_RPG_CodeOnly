#include "AlchemistManager.h"
#include "Item.h"


//開店
bool CAlchemistManager::OpenShop(int _index) {

	if (ShopBank.find(_index) != ShopBank.end()) {

		CItemManager* itemManager = CItemManager::GetInstance();
		ShopMenu.ItemManager = itemManager;

		currentOpenShopIndex = _index;
		ShopMenu.Cursor = 0;
		ShopMenu.IsConfirm = false;

		ShopMenu.ItemList.clear();
		ShopMenu.Basket.clear();
		for (unsigned int i = 0; i < ShopBank[currentOpenShopIndex].size(); i++) {
			ShopMenu.ItemList.push_back(itemManager->GetItem(ShopBank[currentOpenShopIndex][i]));
			ShopMenu.Basket.push_back(0);
		}

		return true;

	} else {
		ERRORDX("Not Found Shop: Key=%d (return BlankList)", _index);
		return false;
	}

}


bool CAlchemistManager::Main() {

	if (!IsOpen()) return false;

	//カーソル移動
	if (CheckHitKeyDown(KEY_INPUT_DOWN)) {
		ShopMenu.Move(DOWN);
	
	} else if (CheckHitKeyDown(KEY_INPUT_UP)) {
		ShopMenu.Move(UP);
	
	}else if (CheckHitKeyDown(KEY_INPUT_RIGHT)) {
		ShopMenu.Move(RIGHT);
	
	} else if (CheckHitKeyDown(KEY_INPUT_LEFT)) {
		ShopMenu.Move(LEFT);
	
	} else if (CheckHitKeyDown(KEY_INPUT_OK)) {
		if (ShopMenu.IsConfirm) {
			ShopMenu.Buy();		//購入
			ShopMenu.IsConfirm = false;
		} else {
			if (ShopMenu.SumPrice <= ShopMenu.ItemManager->GetGold()) {
				ShopMenu.IsConfirm = true;
			}
		}
	
	} else if (CheckHitKeyDown(KEY_INPUT_CANCEL)) {

		if (ShopMenu.IsConfirm) {
			ShopMenu.IsConfirm = false;
		} else if (ShopMenu.SumPrice == 0){
			currentOpenShopIndex = -1;	//閉店
		} else {
			OpenShop(currentOpenShopIndex);	//バスケットリセット
		}
	}

	return IsOpen();
}

/*
void CShopManager::ShopMenu::Move(int _dir) {

	if (!IsConfirm) {

		switch (_dir) {
		case direction_tag::DOWN:
			Cursor = mod(Cursor + 1, ItemList.size());
			break;

		case direction_tag::UP:
			Cursor = mod(Cursor - 1, ItemList.size());
			break;

		case direction_tag::RIGHT:
			if (Cursor < (int)ItemList.size()) {
				Basket[Cursor] =
					min(ItemList[Cursor]->OwnLimit - ItemManager->GetPlayerItemNum(ItemList[Cursor]->Name)
					, Basket[Cursor] + 1);
			}
			break;

		case direction_tag::LEFT:
			Basket[Cursor] = max(0, Basket[Cursor] - 1);
			break;
		}
	}

}

bool CShopManager::ShopMenu::Buy() {

	if (SumPrice <= ItemManager->GetGold()) {

		ItemManager->DecGold(SumPrice);
		for (unsigned int i = 0; i < ItemList.size(); i++) {
			ItemManager->IncPlayerItem(ItemList[i]->Name, Basket[i]);
			Basket[i] = 0;	//バスケットは空にする
		}
		return true;

	} else {
		return false;
	}

}

void CShopManager::ShopMenu::Draw() {

	CRect rect(60, WINDOW_WIDTH - 60, 60, WINDOW_HEIGHT - 60);

	DrawBox(rect, GetColor(30, 20, 80), true);

	//商品リスト
	std::string strNum;
	for (unsigned int i = 0; i < ItemList.size(); i++) {
		int top = rect.Top + 20 + i*(2 + GetFontSize());

		if (Cursor == i && !IsConfirm) {
			DrawString(rect.Left + 20, top, "|>", WHITE, BLACK);
		}

		DrawString(rect.Left + 50, top, ItemList[i]->Name.c_str(), WHITE, BLACK);

		strNum = std::to_string(ItemList[i]->Price) + "ガル";
		DrawString(rect.Right - 240, top, strNum.c_str(), WHITE, BLACK);

		strNum = (Basket[i]>0 ? "< " : "  ") + std::to_string(Basket[i])
			+ (Basket[i] < ItemList[i]->OwnLimit - ItemManager->GetPlayerItemNum(ItemList[i]->Name) ? " >" : "  ");
		DrawString(rect.Right - 120, top, strNum.c_str(), WHITE, BLACK);

		strNum = std::to_string(ItemManager->GetPlayerItemNum(ItemList[i]->Name)) + "/" + std::to_string(ItemList[i]->OwnLimit);
		DrawString(rect.Right - 60, top, strNum.c_str(), WHITE, BLACK);
	}

	//決定
	int top = rect.Bottom - 40;
	if (IsConfirm) {
		DrawString(rect.Left + 30, top, "|>", WHITE, BLACK);
	}
	DrawString(rect.Left + 60, top, "決定", (IsConfirm ? WHITE : GRAY), BLACK);

	//合計金額＆所持金額
	SumPrice = 0;
	for (unsigned int i = 0; i < ItemList.size(); i++) {
		SumPrice += ItemList[i]->Price * Basket[i];
	}
	strNum = "計：" + std::to_string(SumPrice) + "ガル  所持金：" + std::to_string(ItemManager->GetGold()) + "ガル";
	DrawString(rect.Right - 250, top, strNum.c_str(), WHITE, BLACK);

}
*/