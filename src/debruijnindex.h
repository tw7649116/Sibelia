//****************************************************************************
//* Copyright (c) 2012 Saint-Petersburg Academic University
//* All Rights Reserved
//* See file LICENSE for details.
//****************************************************************************

#include "fasta.h"
#include "blockinstance.h"
#include "indexedsequence.h"

#ifndef _DE_BRUIJN_GRAPH_H_
#define _DE_BRUIJN_GRAPH_H_

namespace SyntenyFinder
{	
	class DeBruijnIndex
	{	
	private:
		class BifurcationData;
		typedef std::vector<BifurcationData> BifurcationVector;
	public:		
		static const size_t MAX_POSITION;
		static const size_t MAX_BIFURCATION_ID;		
		static const size_t MAX_SEQUENCE_NUMBER;		
		
		class BifurcationIterator
		{
		public:
			BifurcationIterator();
			bool IsValid() const;
			bool HasNext() const;
			size_t GetProjection() const;
			size_t GetBifurcationId() const;
			BifurcationIterator& operator++();			
			BifurcationIterator operator++(int);
			bool operator == (const BifurcationIterator & it) const;			
		private:
			BifurcationIterator(size_t chrId, size_t index, FastaRecord::Direction dir);
			size_t chrId_;
			size_t index_;
			FastaRecord::Direction dir_;
			const DeBruijnIndex * parent_;
			friend class DeBruijnIndex;
		};
		
		void ApplyChanges();
		DeBruijnIndex(const std::vector<size_t> & revCompTable);
		void GetBifurcationInstances(size_t bifId, std::vector<BifurcationIterator> & ret) const;
		void Replace(BifurcationIterator sourceStart, BifurcationIterator sourceEnd, BifurcationIterator targetStart, BifurcationIterator targetEnd);
		
	private:
		DISALLOW_COPY_AND_ASSIGN(DeBruijnIndex);

		class BifurcationData
		{
		public:
			BifurcationData();						
			BifurcationData(size_t pos, size_t bifId, size_t projection, char inMark, char outMark);
			char GetInMark() const;
			char GetOutMark() const;
			size_t GetProjection() const;
			size_t GetBifurcationId() const;
			size_t GetShift() const;
			bool IsValid() const;
			void Invalidate();
		private:
			uint32_t shift_;
			uint32_t bifId_;
			uint32_t projection_;
			char inMark_;
			char outMark_;
			bool valid_;
			static const uint32_t NO_POSITION;
		};
		
		class Location
		{
		public:
			Location() {}
			Location(size_t chrId, size_t index);
			size_t GetIndex() const;
			size_t GetChromosomeId() const;
		private:
			uint32_t chrId_;
			uint32_t index_;
		};
		
		static size_t GetStrand(FastaRecord::Direction dir);
		typedef std::vector<Location> LocationVector;		

		const std::vector<FastaRecord> * chr_;
		std::vector<size_t> revCompDictionary_;		
		std::vector<LocationVector> bifurcationPlace_;		
		std::vector<BifurcationVector> bifurcationData_;
		friend class BifurcationIterator;		
	};
}

#endif
