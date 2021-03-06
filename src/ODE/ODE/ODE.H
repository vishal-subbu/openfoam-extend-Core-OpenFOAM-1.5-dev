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
    ODE

Description
    Abstract base class for the ODE solvers.

\*---------------------------------------------------------------------------*/

#ifndef ODE_H
#define ODE_H

#include "scalarField.H"
#include "Matrix.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class ODE Declaration
\*---------------------------------------------------------------------------*/

class ODE
{
public:

    // Constructors

        //- Construct null
        ODE()
        {}


    // Destructor

        virtual ~ODE()
        {}


    // Member Functions

        //- Return number of equations
        virtual label nEqns() const = 0;

        //- Return access to coefficients
        virtual scalarField& coeffs() = 0;

        //- Return reference to coefficients
        virtual const scalarField& coeffs() const = 0;

        //- Evaluate derivatives
        virtual void derivatives
        (
            const scalar x,
            const scalarField& y,
            scalarField& dydx
        ) const = 0;

        //- Evaluate Jacobian
        virtual void jacobian
        (
            const scalar x,
            const scalarField& y,
            scalarField& dfdx,
            Matrix<scalar>& dfdy
        ) const = 0;

        //- Update ODE after the solution, advancing by delta
        virtual void update(const scalar delta) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
