#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/SongInfoLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();
		auto mainLayer = this->getChildByID("main-layer");
		auto leftMenu = mainLayer->getChildByID("left-button-menu");
		auto rightMenu = mainLayer->getChildByID("right-button-menu");
		auto settingPos = Mod::get()->getSettingValue<std::string>("position");
		auto targetMenu = (settingPos == "Left") ? leftMenu : rightMenu;

		if (!targetMenu) {
			targetMenu = leftMenu;
			return;
		}

		auto sprlabel = CCSprite::create("songspr.png"_spr);
		auto spr = CircleButtonSprite::create(sprlabel, geode::CircleBaseColor::Green, geode::CircleBaseSize::Medium);
		sprlabel->setScale(1.0f);
		sprlabel->setPositionX(24);
		spr->setScale(0.6f);
		auto btn = CCMenuItemSpriteExtra::create(
			spr, this, menu_selector(MyPauseLayer::onSongInfoButton)
		);

		targetMenu->addChild(btn, 0);
		targetMenu->updateLayout();
	}

	void onSongInfoButton(CCObject* sender) {
		auto pl = PlayLayer::get();
		auto lvl = pl->m_level;
		auto songID = lvl->m_songID;
		auto mdm = MusicDownloadManager::sharedState();
		auto songinfo = mdm->getSongInfoObject(songID);

		if (!songinfo) {
			Notification::create("No song info available!", NotificationIcon::Error)->show();
			return;
		}

		auto sil = SongInfoLayer::create(songinfo);
		sil->show();
	}
};
