					; fixed code
(include "CANDO/db/amideBondTester.l" )
(include "CANDO/db/stereochemistryTester.l" ) ;
					;
					; A GeometryChecker object will carry out a series of tests
					; on a molecules conformation usually generated by a stochastic search
					;
					; When you set it up you give it a Matter and it will identify groups to watch
					; like amide bonds and then subsequent calls will quickly check if those groups have
					; good geometry
					;

(defClass GeometryChecker ()(_AmideTester _Matter )
  (method __init__ (self matter )
	  (setq (slot self _Matter )matter )
	  (setq (slot self _AmideTester )(AmideBondTester matter )))
  (method hasGoodGeometry (self energyFunction matter problems fails )
	  (let (beyondThreshold badStereocenters badAmides )
	    (setq beyondThreshold (checkForBeyondThresholdInteractions energyFunction ))
	    (when (> beyondThreshold 0 )
	      (writeln problems (% "STOCHASTIC-FAIL;%3d: !!! There were %d beyond threshold interactions" fails beyondThreshold ))
	      (return-from hasGoodGeometry false ))
	    (setq badStereocenters (countBadStereocenters matter problems ))
	    (ifTrue (> badStereocenters 0 )
		    (writeln problems (% "STOCHASTIC-FAIL;%3d: !!! There were %d bad stereocenters (centers with the wrong configuration) " fails badStereocenters ))
		    (return-from hasGoodGeometry false ))
	    (setq badAmides (countBadAmidesInMatter (slot self _AmideTester )matter ))
	    (ifTrue (> badAmides 0 )
		    (writeln problems (% "STOCHASTIC-FAIL;%3d: !!! There were %d bad amide bonds [cis acyclic amides + trans cyclic amides]" fails badAmides ))
		    (return-from hasGoodGeometry false ))
	    true ))   ; true - The conformation has good geometry
)