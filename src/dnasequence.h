#ifndef _DNA_SEQUENCE_H_
#define _DNA_SEQUENCE_H_

#include "common.h"

#pragma warning(disable:4355)

namespace SyntenyBuilder
{
	class DNASequence
	{
	public:
		enum Direction
		{
			positive,
			negative
		};

		typedef unsigned int Pos;
		struct DNACharacter
		{
			char actual;
			Pos pos;
			DNACharacter() {}
			DNACharacter(char actual): actual(actual), pos(actual) {}
			DNACharacter(char actual, Pos pos): actual(actual), pos(pos) {}
		};

		typedef std::list<DNACharacter> Sequence;
		typedef Sequence::iterator SequencePosIterator;
		typedef Sequence::reverse_iterator SequenceNegIterator;

	private:
		static const size_t NO_POS;

		class GenericIterator
		{
		public:
			virtual ~GenericIterator();
			virtual char Spell() const = 0;			
			virtual void MoveForward() = 0;
			virtual void MoveBackward() = 0;
			virtual char TranslateChar(char ch) const = 0;
			virtual GenericIterator* Clone() const = 0;
			virtual Direction GetDirection() const = 0;
			virtual const DNACharacter* GetNaked() const = 0;
			virtual GenericIterator* Invert() const = 0;
			virtual SequencePosIterator Base() const = 0;
			virtual bool Equal(const GenericIterator& toCompare) const = 0;
		};

		class ForwardIterator: public GenericIterator
		{
		public:
			char Spell() const;
			void MoveForward();
			void MoveBackward();
			char TranslateChar(char ch) const;
			Direction GetDirection() const;
			GenericIterator* Clone() const;
			GenericIterator* Invert() const;
			SequencePosIterator Base() const;
			const DNACharacter* GetNaked() const;			
			bool Equal(const GenericIterator& toCompare) const;
			ForwardIterator();
			ForwardIterator(SequencePosIterator it);
		private:
			SequencePosIterator it_;
		};

		class BackwardIterator: public GenericIterator
		{
		public:
			char Spell() const;
			void MoveForward();
			void MoveBackward();
			char TranslateChar(char ch) const;
			Direction GetDirection() const;
			GenericIterator* Clone() const;
			GenericIterator* Invert() const;
			SequencePosIterator Base() const;
			const DNACharacter* GetNaked() const;
			bool Equal(const GenericIterator& toCompare) const;
			BackwardIterator();
			BackwardIterator(SequenceNegIterator it);
		private:
			SequenceNegIterator it_;
		};

	public:			
		class StrandIterator: public std::iterator<std::bidirectional_iterator_tag, char, size_t>
		{
		public:
			void MakeInverted();
			void Swap(StrandIterator & toSwap);
			char operator * () const;
			StrandIterator Invert() const;
			Direction GetDirection() const;
			StrandIterator& operator ++ ();
			StrandIterator operator ++ (int);
			StrandIterator& operator -- ();
			StrandIterator operator -- (int);
			size_t GetElementId() const;
			size_t GetOriginalPosition() const;
			SequencePosIterator Base() const;
			char TranslateChar(char ch) const;
			const DNACharacter* GetNaked() const;
			bool operator < (const StrandIterator & comp) const;
			bool operator == (const StrandIterator & comp) const;
			bool operator != (const StrandIterator & comp) const;
			StrandIterator();
			StrandIterator(GenericIterator * it);
			StrandIterator(const StrandIterator & toCopy);
			StrandIterator(SequencePosIterator base, Direction direction);
			StrandIterator& operator = (const StrandIterator & toCopy);
		private:
			std::auto_ptr<GenericIterator> it_;
		};
		
		size_t Size() const;
		StrandIterator PositiveBegin() const;
		StrandIterator PositiveEnd() const;
		StrandIterator NegativeBegin() const;
		StrandIterator NegativeEnd() const;
		void EraseN(StrandIterator now, size_t count);		
		void Replace(StrandIterator source, size_t sourceDistance, 
			StrandIterator target, size_t targetDistance,
			const boost::function<void (const StrandIterator&)> & alarmBefore,
			const boost::function<void (const StrandIterator&)> & alarmAfter);
		void CopyN(StrandIterator source, size_t count, StrandIterator target);
		explicit DNASequence(const std::string & sequence);

		template<class Iterator>
			std::pair<size_t, size_t> SpellOriginal(StrandIterator it1, StrandIterator it2, Iterator out) const
			{
				for(;it1.GetOriginalPosition() == NO_POS && it1 != it2; ++it1);
				for(--it2; it2.GetOriginalPosition() == NO_POS && it1 != it2; --it2);
				size_t start = std::min(it1.GetOriginalPosition(), it2.GetOriginalPosition());
				size_t end = std::max(it1.GetOriginalPosition(), it2.GetOriginalPosition()) + 1;

				if(start == NO_POS || end == NO_POS)
				{
					return std::make_pair(0, 0);
				}

				if(it1.GetDirection() == positive)
				{
					std::copy(original_.begin() + start, original_.begin() + end, out);
				}
				else
				{
					std::string rcomp;
					std::copy(original_.begin() + start, original_.begin() + end, std::back_inserter(rcomp));
					for(size_t i = 0; i < rcomp.size(); i++)
					{
						rcomp[i] = Translate(rcomp[i]);
					}

					std::copy(rcomp.rbegin(), rcomp.rend(), out);
				}

				return std::make_pair(start, end);
			}
		
		static const std::string alphabet;
	private:
		DISALLOW_COPY_AND_ASSIGN(DNASequence);	
		static char Translate(char ch);
		static const std::string complementary_;		
		Sequence sequence_;
		std::string original_;
	};	

	inline bool Valid(const DNASequence::StrandIterator & it, const DNASequence & sequence)
	{
		return it != sequence.PositiveEnd() && it != sequence.NegativeEnd();
	}

	inline bool AtBegin(const DNASequence::StrandIterator & it, const DNASequence & sequence)
	{
		return it == sequence.PositiveBegin() || it == sequence.NegativeBegin();
	}

	inline bool ProperKMer(DNASequence::StrandIterator it, const DNASequence & sequence, size_t k)
	{
		DNASequence::StrandIterator upperBound = it.GetDirection() == 
			DNASequence::positive ? sequence.PositiveBegin() :sequence.NegativeBegin();
		return Valid(AdvanceForward(it, upperBound, k - 1), sequence);
	}
}

#endif
