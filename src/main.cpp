#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/SongInfoLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();
		auto leftMenu = this->getChildByID("left-button-menu");
		auto rightMenu = this->getChildByID("right-button-menu");
		auto settingPos = Mod::get()->getSettingValue<std::string>("position");
		auto targetMenu = (settingPos == "Left") ? leftMenu : rightMenu;

		if (!targetMenu) {
			targetMenu = leftMenu;
			return;
		}

		auto spr = CCSprite::createWithSpriteFrameName("edit_eSongBtn_001.png");
		spr->setScale(0.8f);
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
		auto ngUrl = "https://www.newgrounds.com/audio/listen/" + std::to_string(songinfo->m_songID);
		auto fixedYTUrl = "https://www.youtube.com/channel/" + songinfo->m_youtubeChannel;
		auto ngArtistUrl = "https://" + songinfo->m_artistName + ".newgrounds.com";
		SongInfoLayer::create(
			songinfo->m_songName,
			songinfo->m_artistName,
			ngUrl,
			ngArtistUrl,
			fixedYTUrl, 
			"",
			songID,
			"",
			songinfo->m_nongType
		)->show();
	}
};
