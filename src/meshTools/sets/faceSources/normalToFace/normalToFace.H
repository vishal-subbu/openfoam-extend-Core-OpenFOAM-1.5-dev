/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    Foam::normalToFace

Description
    A topoSetSource to select faces based on normal.

SourceFiles
    normalToFace.C

\*---------------------------------------------------------------------------*/

#ifndef normalToFace_H
#define normalToFace_H

#include "topoSetSource.H"
#include "NamedEnum.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                       Class normalToFace Declaration
\*---------------------------------------------------------------------------*/

class normalToFace
:
    public topoSetSource
{

private:

        //- Add usage string
        static addToUsageTable usage_;

        //- (unit)vector to compare to
        vector normal_;

        //- Tolerance (i.e. cos of angle between normal_ and faceNormal)
        const scalar tol_;


    // Private Member Functions

        //- Normalize normal and check tolerance
        void setNormal();


public:

    //- Runtime type information
    TypeName("normalToFace");

    // Constructors

        //- Construct from components
        normalToFace
        (
            const polyMesh& mesh,
            const vector& normal,
            const scalar tol
        );

        //- Construct from dictionary
        normalToFace(const polyMesh& mesh, const dictionary& dict);

        //- Construct from Istream
        normalToFace(const polyMesh& mesh, Istream&);


    // Destructor

        virtual ~normalToFace();


    // Member Functions

        virtual void applyToSet
        (
            const topoSetSource::setAction action,
            topoSet&
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
