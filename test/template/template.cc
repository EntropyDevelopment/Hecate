/*	Copyright 2016 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#include <gtest/gtest.h>
#include "Template.hh"
#include "Character.hh"

using namespace Entropy::Hecate;
using namespace testing;
using namespace std;

namespace {
	ENTROPY_HECATE_DEFINE_STAT(St, 2);
	ENTROPY_HECATE_DEFINE_STAT(Dx, 2);
	ENTROPY_HECATE_DEFINE_STAT(Iq, 2);
	ENTROPY_HECATE_DEFINE_STAT(Ht, 2);

	ENTROPY_HECATE_DEFINE_SKILL(Attack, 1, St, St, Dx);
	ENTROPY_HECATE_DEFINE_SKILL(Defense, 1, Dx, Dx, Ht);
	ENTROPY_HECATE_DEFINE_SKILL(Research, 1, Iq);
	ENTROPY_HECATE_DEFINE_SKILL(Experiment, 1, Research, Iq);

	ENTROPY_HECATE_DEFINE_CHARACTER(St, Dx, Iq, Ht);

	class Character :
		public BaseCharacter
	{
		public:
			template<typename ...sl>
			Character(const sl & ...);
			ENTROPY_HECATE_STAT_ACCESSOR(Strength, St);
	};

	using Trait = Trait<BaseCharacter>;
	using Template = Template<BaseCharacter>;
	using TraitHolder = TraitHolder<BaseCharacter>;

	template<typename T>
	using SkillHolder = SkillHolder<BaseCharacter, T>;

	TEST(Template, Create) {
		St st = 0;
		Dx dx = 0;

		Template empty("empty template"s, {});
		Template stat("skill template"s, {
			make_shared<SkillHolder<Attack>>(Attack(10, st, st, dx))
		});
		Template trait("trait template"s, {
			make_shared<TraitHolder>(Trait("held trait"s, 20, [](auto &, auto &){}))
		});
		Template many("many template"s, {
			make_shared<SkillHolder<Attack>>(Attack(10, st, st, dx)),
			make_shared<TraitHolder>(Trait("held trait"s, 20, [](auto &, auto &){}))
		});

		EXPECT_EQ(empty.size(), 0);
		EXPECT_EQ(stat.size(), 1);
		EXPECT_EQ(trait.size(), 1);
		EXPECT_EQ(many.size(), 2);
	}

	TEST(Template, Add) {
		try
		{
			St st = 0;
			Dx dx = 0;

			Character test(0, 0, 0, 0);
			Attack att(test.Stats());

			test.get(att);

			Template many("many template"s, {
				make_shared<SkillHolder<St>>(St(10)),
				make_shared<SkillHolder<Attack>>(Attack(10, st, st, dx)),
				make_shared<TraitHolder>(Trait("rage", 20, [](auto &ev, auto &ch) {
					if(ev.Id() == 11)
						get<St>(ch.Stats()).Raw() += 10;
					else if(ev.Id() == 12)
						get<St>(ch.Stats()).Raw() -= 10;
				}))
			});

			EXPECT_EQ(test.Cost(), 0);
			EXPECT_EQ(many.size(), 3);

			test.Add(many);
			EXPECT_EQ(test.Strength().Value(), 10);
			EXPECT_EQ(test.get(att).Value(), 16);
			EXPECT_EQ(test.Cost(), 50);

			Event up(11);
			Event down(12);

			test(up);
			EXPECT_EQ(test.Strength().Value(), 20);
			EXPECT_EQ(test.Cost(), 70);

			test(down);
			EXPECT_EQ(test.Strength().Value(), 10);
			EXPECT_EQ(test.Cost(), 50);

			test.Remove(many);
		}
		catch(exception &e)
		{
			FAIL() << e << endl;
		}
	}

	template<typename ...sl>
	Character::Character(const sl & ...s)
		: BaseCharacter(s...)
	{}
}