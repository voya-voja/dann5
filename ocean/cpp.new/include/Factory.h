#pragma once

#include <map>

using namespace std;

namespace dann5 {
	namespace ocean {

		template <typename K, typename T>
		class Factory
		{
		public:
			static Factory<K, T>& Instance() 
			{ 
				return gFactory; 
			}

			T* create(K key)
			{
				return mCreators[key]->create();
			}


			template<typename K, typename T>
			class Creator
			{
			public:
				Creator(K key)
					:mKey(key)
				{
					Factory<K, T>::Instance().add(this);
				}

				virtual T* create() = 0;

				K key() { return mKey; }

			protected:
			private:
				K mKey;
			};

		protected:
		private:
			Factory() {}
			~Factory()
			{
#if FACTORY_DELETE_CREATORS
				std::map<K, CreatorB<K, T>*>::template iterator at(mCreators.begin());
				for (; at != mCreators.end(); at++)
				{
					CreatorB<K, T>* pCreator = at->second;
					try { delete pCreator; } // delete allocated Creators
					catch (...) {}	// otherwise ignor those that are not allocated
				}
#endif
			}

			void add(Creator<K, T>* creator)
			{
				mCreators[creator->key()] = creator;
			}

			static Factory<K, T>			gFactory;
			std::map<K, Creator<K, T>*>	mCreators;
		};

		template<typename K, typename T, typename O>
		class Creator : Factory<K,T>::Creator<K, T>
		{
		public:
			Creator(K key)
				:Factory<K,T>::template Creator<K,T>(key)
			{}

			virtual T* create()
			{
				return(new O());
			}

		protected:
		private:
		};

	};
};