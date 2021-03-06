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
    Foam::ThermoParcel

Description
    Thermodynamic parcel class with one/two-way coupling with the continuous
    phase. Includes Kinematic parcel sub-models, plus:
    - heat transfer

SourceFiles
    ThermoParcelI.H
    ThermoParcel.C
    ThermoParcelIO.C

\*---------------------------------------------------------------------------*/

#ifndef ThermoParcel_H
#define ThermoParcel_H

#include "IOstream.H"
#include "autoPtr.H"
#include "interpolationCellPoint.H"
#include "contiguous.H"

#include "KinematicParcel.H"
#include "ThermoCloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

template<class ParcelType>
class ThermoParcel;

template<class ParcelType>
Ostream& operator<<
(
    Ostream&,
    const ThermoParcel<ParcelType>&
);

/*---------------------------------------------------------------------------*\
                       Class ThermoParcel Declaration
\*---------------------------------------------------------------------------*/

template<class ParcelType>
class ThermoParcel
:
    public KinematicParcel<ParcelType>
{

public:

    //- Class to hold thermo particle constant properties
    class constantProperties
    :
        public KinematicParcel<ParcelType>::constantProperties
    {

        // Private data

            //- Particle initial temperature [K]
            const scalar T0_;

            //- Particle specific heat capacity [J/(kg.K)]
            const scalar cp0_;

            //- Particle emissivity [] (radiation)
            const scalar epsilon0_;

            //- Particle scattering factor [] (radiation)
            const scalar f0_;


    public:

        // Constructors
        constantProperties(const dictionary& dict);

        // Member functions

            // Access

                //- Return const access to particle initial temperature [K]
                inline scalar T0() const;

                //- Return const access to particle specific heat capacity
                //  [J/(kg.K)]
                inline scalar cp0() const;

                //- Return const access to Particle emissivity [] (radiation)
                inline scalar epsilon0() const;

                //- Return const access to Particle scattering factor []
                //  (radiation)
                inline scalar f0() const;
    };


    //- Class used to pass thermo tracking data to the trackToFace function
    class trackData
    :
        public KinematicParcel<ParcelType>::trackData
    {

        // Private data

            //- Reference to the cloud containing this particle
            ThermoCloud<ParcelType>& cloud_;

            //- Particle constant properties
            const constantProperties& constProps_;

            // Interpolators for continuous phase fields

                //- Temperature field interpolator
                const interpolation<scalar>& TInterp_;

                //- Scpecific heat capacity field interpolator
                const interpolation<scalar>& cpInterp_;


    public:

        // Constructors

            //- Construct from components
            inline trackData
            (
                ThermoCloud<ParcelType>& cloud,
                const constantProperties& constProps,
                const interpolation<scalar>& rhoInterp,
                const interpolation<vector>& UInterp,
                const interpolation<scalar>& muInterp,
                const interpolation<scalar>& TInterp,
                const interpolation<scalar>& cpInterp,
                const vector& g
            );


        // Member functions

            //- Return access to the owner cloud
            inline ThermoCloud<ParcelType>& cloud();

            //- Return const access to the owner cloud
            inline const constantProperties& constProps() const;

            //- Return conat access to the interpolator for continuous
            //  phase temperature field
            inline const interpolation<scalar>& TInterp() const;

            //- Return conat access to the interpolator for continuous
            //  phase specific heat capacity field
            inline const interpolation<scalar>& cpInterp() const;
    };


protected:

    // Protected data

        // Parcel properties

            //- Temperature [K]
            scalar T_;

            //- Specific heat capacity [J/(kg.K)]
            scalar cp_;


        // Call-based quantities

            //- Temperature [K]
            scalar Tc_;

            //- Specific heat capacity [J/(kg.K)]
            scalar cpc_;


    // Protected member functions

        //- Calculate new particle temperature
        template<class TrackData>
        scalar calcHeatTransfer
        (
            TrackData& td,
            const scalar dt,
            const label celli,
            scalar& htc,
            scalar& dhTrans
        );


public:

    //- Runtime type information
    TypeName("ThermoParcel");

    friend class Cloud<ParcelType>;


    // Constructors

        //- Construct from components
        inline ThermoParcel
        (
            ThermoCloud<ParcelType>& owner,
            const label typeId,
            const vector& position,
            const label celli,
            const scalar d0,
            const vector& U0,
            const scalar nParticle0,
            const constantProperties& constProps
        );

        //- Construct from Istream
        ThermoParcel
        (
            const Cloud<ParcelType>& c,
            Istream& is,
            bool readFields = true
        );

        //- Construct and return a clone
        autoPtr<ParcelType> clone() const
        {
            return autoPtr<ParcelType>(new ThermoParcel<ParcelType>(*this));
        }


    // Member Functions

        // Access

            //- Return const access to temperature
            inline scalar T() const;

            //- Return const access to specific heat capacity
            inline scalar cp() const;


        // Edit

            //- Return access to temperature
            inline scalar& T();

            //- Return access to specific heat capacity
            inline scalar& cp();


        // Main calculation loop

            //- Update cell based quantities
            template<class TrackData>
            void updateCellQuantities
            (
                TrackData& td,
                const scalar dt,
                const label celli
            );

            //- Coupled calculation with the continuous phase
            template<class TrackData>
            void calcCoupled
            (
                TrackData& td,
                const scalar dt,
                const label celli
            );

            //- Uncoupled calculation with the continuous phase
            template<class TrackData>
            void calcUncoupled
            (
                TrackData& td,
                const scalar dt,
                const label
            );


        // I-O

            static void readFields(ThermoCloud<ParcelType>& c);

            static void writeFields(const ThermoCloud<ParcelType>& c);


    // Ostream Operator

        friend Ostream& operator<< <ParcelType>
        (
            Ostream&,
            const ThermoParcel<ParcelType>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "ThermoParcelI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "ThermoParcel.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

