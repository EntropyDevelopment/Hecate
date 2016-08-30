/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_HECATE_CHECK_INC
#	define ENTROPY_HECATE_CHECK_INC

#	include <memory>
#	include <list>

#	include "Modifier.hh"

#	ifndef ENTROPY_HECATE_DEFAULT_LUCK
#		define ENTROPY_HECATE_DEFAULT_LUCK 5
#	endif

	namespace Entropy
	{
		namespace Hecate
		{
			class Check
			{
				public:
					class Result
					{
						private:
							struct result_modifier
							{
								Percent current;
								std::shared_ptr<Modifier> modifier;
							};
						public:
							Result(const int, const Percent, const std::list<std::shared_ptr<Modifier>> &);
							int Value() const;
							Percent Luck() const;
							std::size_t size() const;
							std::list<result_modifier>::iterator begin();
							std::list<result_modifier>::iterator end();
						private:
							int _value;
							Percent _luck;
							std::list<result_modifier> _modifiers;
					};
				public:
					Check(std::list<std::shared_ptr<Modifier>>);
					Check(Percent &, std::list<std::shared_ptr<Modifier>>);
					template<typename ...mods>
					Result operator () (const mods &...) const;
				private:
					static Percent default_luck;
					std::list<std::shared_ptr<Modifier>> _modifiers;
					Percent &_luck;
			};
		}
	}

#	include "Check.impl.hh"

#endif
