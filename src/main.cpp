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

		auto sprlabel = CCSprite::create("songspr.png"_spr);
		auto spr = CircleButtonSprite::create(sprlabel, geode::CircleBaseColor::Green, geode::CircleBaseSize::Medium);
		sprlabel->setScale(1.0f);
		sprlabel->setPositionX(24);
		spr->setScale(0.7f);
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

		auto ngSongUrl = "https://www.newgrounds.com/audio/listen/" + std::to_string(songID);
		auto fixedYTUrl = "https://www.youtube.com/channel/" + songinfo->m_youtubeChannel;
		auto ngArtistUrl = "https://" + songinfo->m_artistName + ".newgrounds.com";
		if (songID > 10000000) {
			ngSongUrl = songinfo->m_songUrl.c_str();
		}
		auto sil = SongInfoLayer::create(
			songinfo->m_songName,
			songinfo->m_artistName,
			ngSongUrl,
			ngArtistUrl,
			fixedYTUrl, 
			"",
			songID,
			"",
			songinfo->m_nongType
		);
		sil->show();
		auto cclayer = sil->getChildByType<CCLayer>(0);
		if (!cclayer) {
			return;
		}
		auto ccmenu = cclayer->getChildByType<CCMenu>(0);
		if (!ccmenu) {
			return;
		}
		auto ytspr = ccmenu->getChildByType<CCMenuItemSpriteExtra>(-1);
		if (!ytspr) {
			return;
		}
		if (ngSongUrl != "https://ncs.io") {
			if (songinfo->m_youtubeChannel.empty()) {
				ytspr->setVisible(false);
				auto ngspr = ccmenu->getChildByType<CCMenuItemSpriteExtra>(-2);
				if (!ngspr) {
					return;
				}
				ngspr->setPositionX(200);
			}
			if (songinfo->m_songUrl.empty()) {
				auto webspr = ccmenu->getChildByType<CCMenuItemSpriteExtra>(-2);
				if (!webspr) {
					return;
				}
				webspr->setVisible(false);
				ytspr->setPositionX(200);
			}
		}
	}
};
