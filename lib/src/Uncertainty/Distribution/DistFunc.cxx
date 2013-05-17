//                                               -*- C++ -*-
/**
 *  @file  DistFunc.cxx
 *  @brief OpenTURNS wrapper to a library of special functions
 *
 *  Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author schueller
 *  @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "DistFunc.hxx"
#include "RandomGenerator.hxx"
#include "Log.hxx"
#include "SquareMatrix.hxx"
#include "Normal2DCDF.hxx"
#include "Normal3DCDF.hxx"
#include "Exception.hxx"
#include "Tvpack.hxx"

#define USE_DCDFLIB
#ifdef USE_DCDFLIB
#include "Dcdflib.hxx"
#endif

#include "SpecFunc.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS




const UnsignedLong    DistFunc::NumberOfBandNormalZigurrat = 129;
const NumericalScalar DistFunc::NormalZigguratTail = 3.44508288805539135654449538289;
const NumericalScalar DistFunc::NormalZigguratAbscissa[DistFunc::NumberOfBandNormalZigurrat + 1] =
  {
    0.0                             , 0.271599048510693754998163050702, 0.361898145612134821317157669025, 0.425392047157312675983415424660,
    0.476131717888617028166988513439, 0.519219013293501476350033678035, 0.557137093511580298809061311694, 0.591298984443313319787098187809,
    0.622592649542783642315521009315, 0.651616031063230052452116746597, 0.678792877347767760686380971868, 0.704435546364376654764501920124,
    0.728781608504968321114578996184, 0.752016422469955992826848889422, 0.774287710877425340184686032696, 0.795715332287609618751456615712,
    0.816398043479172731605919440849, 0.836418306930649053279379059005, 0.855845789056994122927551923075, 0.874739957924955634206040955649,
    0.893152046999867275098199151897, 0.911126563328297873672484801050, 0.928702462334730770821130245805, 0.945914074632515687029155431559,
    0.962791845646485921191452370469, 0.979362932051085821966553188665, 0.995651687354039595085786832964, 1.01168006070531516232965392900 ,
    1.02746792709172237058780318928 , 1.04303336277130264040166296783 , 1.05839287662899292124745211169 , 1.07356160576994269038108408924 ,
    1.08855348188453823734559909469 , 1.10338137356245140332933074704 , 1.11805720869053844255517390583 , 1.13259208026135637042889232625 ,
    1.14699633828758735801507046149 , 1.16127967002039429336339539399 , 1.17545117027533417287473048372 , 1.18951940335453972086012397336 ,
    1.20349245780083403835096451764 , 1.21737799501490321160206827336 , 1.23118329260039645151126944958 , 1.24491528316597898248595200283 ,
    1.25858058920182308169327058845 , 1.27218555455602081689118701416 , 1.28573627296019479845973032796 , 1.29923861399021398514811910954 ,
    1.31269824679504233415845967597 , 1.32612066188248490208347357933 , 1.33951119121344617033356972221 , 1.35287502682506865937549704692 ,
    1.36621723817679783092212387778 , 1.37954278839122803847614338568 , 1.39285654954287803582443862287 , 1.40616331713229672637906532114 ,
    1.41946782386968059570797384556 , 1.43277475288114273351645465423 , 1.44608875044162355949759042785 , 1.45941443833094318522780953546 ,
    1.47275642590347769759399918804 , 1.48611932195724762500809972081 , 1.49950774648472030334842211111 , 1.51292634238526126953827654531 ,
    1.52637978721786115033208986070 , 1.53987280507247501439918234146 , 1.55341017863902384055596007811 , 1.56699676155482666996780634172 ,
    1.58063749111398211301308134969 , 1.59433740142604557618725800299 , 1.60810163711632290631602733261 , 1.62193546766631570500142829940 ,
    1.63584430250042915164619942820 , 1.64983370693414137939082821110 , 1.66390941910962125167951454089 , 1.67807736805749698528091063361 ,
    1.69234369303839820145325283038 , 1.70671476433535309787605152682 , 1.72119720568852405198043944381 , 1.73579791858759592141174442291 ,
    1.75052410866497944355179477982 , 1.76538331446556845629758885967 , 1.78038343890695608962355720514 , 1.79553278378881937566665179110 ,
    1.81084008776290159625982126988 , 1.82631456823722069305589567937 , 1.84196596776173086857556181334 , 1.85780460553003206309142077930 ,
    1.87384143473579289060235282188 , 1.89008810664697886815311451817 , 1.90655704241032628459974673316 , 1.92326151377851307313333422169 ,
    1.94021573417040550238786161927 , 1.95743496173982213662503222153 , 1.97493561645224006631535036596 , 1.99273541356693511100128687843 ,
    2.01085351641383291996867861457 , 2.02931071196545767371544926885 , 2.04812961346843841880165860053 , 2.06733489536060750602383910558 ,
    2.08695356691825680949271767439 , 2.10701529263367409420415792810 , 2.12755276932450072216751946158 , 2.14860217257314763105233505342 ,
    2.17020368849378343938267967922 , 2.19240215131716394363476960255 , 2.21524781328299046507842208032 , 2.23879728143015418864904200351 ,
    2.26311466694490835351573544076 , 2.28827300805157643267084011891 , 2.31435604894699812266939486476 , 2.34146048795978434043531292659 ,
    2.36969885260647551209738645471 , 2.39920322494817055544917817450 , 2.43013013978464913728162829644 , 2.46266713119979698270547205291 ,
    2.49704164517949055368308433652 , 2.53353343078795745430738206406 , 2.57249218737695205289488067611 , 2.61436340874381837128188016042 ,
    2.65972749227020134812347957976 , 2.70936127691136762243924804229 , 2.76433956507998809402134093896 , 2.82621272644033894046564603318 ,
    2.89734153514829733018851271394 , 2.98159419140365968437694926385 , 3.08601280427975485670709771297 , 3.22573136821851522103605577983 ,
    3.44508288805539135654449538289 , 3.71537970891694553086615539964
  };

const NumericalScalar DistFunc::NormalZigguratRatio[DistFunc::NumberOfBandNormalZigurrat] =
  {
    0.0                             , 0.750484775353838553484969850043, 0.850740271310955168539797476445, 0.893433541969631939167066002439,
    0.917015181837094471851335493153, 0.931941203234046233441298028733, 0.942225689827803086945282581056, 0.949736532992396229158449917333,
    0.955459380775071654971678008833, 0.959962976643589437257667346996, 0.963598275031758611303047864447, 0.966593473467949497522283939333,
    0.969103315737874152720557392852, 0.971236417555650662146408519772, 0.973071247290683962507094254337, 0.974665898140297597377304564176,
    0.976064293086860561017929573224, 0.977300253883645249170475123945, 0.978400244899316179173188425569, 0.979385269129977846895884043302,
    0.980272206900904494815390564396, 0.981074779362329241400634997295, 0.981804253938740083291033345358, 0.982469968882383589521668758583,
    0.983079728809119850562796554795, 0.983640106766412054356146072398, 0.984156677615944084392929857459, 0.984634200280007552311796735515,
    0.985076761458307086707253212313, 0.985487889991653471429663256532, 0.985870648634019490358385815715, 0.986227708271493343086583402375,
    0.986561408382272362985077361984, 0.986873806622761920341336293274, 0.987166719753625799389459567589, 0.987441757619090675912393772106,
    0.987700351516051174211224554768, 0.987943778003453440265050048019, 0.988173178983434816520165815368, 0.988389578716739479383699175238,
    0.988593898303624926534040096469, 0.988786968058724293246032191279, 0.988969538127397455898436508912, 0.989142287626960404173170844597,
    0.989305832545044324977737622638, 0.989460732586336934407985727862, 0.989607497125912170683173044980, 0.989746590400581327272272451975,
    0.989878436047901646747803160708, 0.990003421084648827869197604239, 0.990121899401909443020286447879, 0.990234194841858232813880454444,
    0.990340603911263982506239462637, 0.990441398178427316016322972125, 0.990536826393283914925721684389, 0.990627116364558512218324337623,
    0.990712476622928023181406791844, 0.990793097894984079572175093174, 0.990869154409244076145976128873, 0.990940805052437824691268287474,
    0.991008194391705548082954937064, 0.991071453576109215637707296064, 0.991130701128922528307152229819, 0.991186043640474608746669498017,
    0.991237576369836074033189634373, 0.991285383762317554082871105898, 0.991329539888568903748275039752, 0.991370108809995454035219189473,
    0.991407144874221943150024047055, 0.991440692943413945013043999353, 0.991470788557391074579718311726, 0.991497458032617503483410551001,
    0.991520718497315401540555107636, 0.991540577862097828628588285611, 0.991557034724640724341601679445, 0.991570078205989141038468359572,
    0.991579687715098995526722137843, 0.991585832637127947522049524329, 0.991588471939779518821684067585, 0.991587553690640507148992013935,
    0.991583014476894319513703858007, 0.991574778716995497249215923087, 0.991562757851797002828992213523, 0.991546849400162799407295751574,
    0.991526935861188886574791544186, 0.991502883441690831444489168144, 0.991474540583463335923580694623, 0.991441736259812196096950028863,
    0.991404278004792265983740672627, 0.991361949631191261607889802980, 0.991314508584238996353362687132, 0.991261682866858262508333044405,
    0.991203167458441781253777050367, 0.991138620131902842410876661610, 0.991067656552139074390092217899, 0.990989844511792319927328343055,
    0.990904697125582941945109300521, 0.990811664760263111407350456154, 0.990710125420284608160345942834, 0.990599373235399951429113359060,
    0.990478604599807611123088277090, 0.990346901385037190355276297326, 0.990203210479193398963460161789, 0.990046318677290513694722835952,
    0.989874821637971854574784332163, 0.989687085197041986702889368351, 0.989481196737866229216740827939, 0.989254903487686985183950871399,
    0.989005533422740516025904697311, 0.988729892746066823168948592841, 0.988424131369219266125861236354, 0.988083564029399232263953634600,
    0.987702428858508882825722518646, 0.987273556123533683467427764224, 0.986787905274353495741507623455, 0.986233904410023291038018271182,
    0.985596485459788251551070665927, 0.984855636576794042676975880029, 0.983984161793717454104724720040, 0.982944085941803527491004884902,
    0.981680632603656288469381808499, 0.980111601026471711171610577703, 0.978107394117398550145114447318, 0.975450319596402797690182574081,
    0.971742413337712360174408714492, 0.966163908091604482763906551568, 0.956686237014236193571274296719, 0.936329102386070277297433094023,
    0.927249206800360320095099544455
  };

/**************************************************************************************************************************/
/* Normalized Beta distribution, i.e. with a PDF equals to x ^ (p1 - 1) . (1 - x) ^ (p2 - 1) / Beta(p1, p2) . (0 < x < 1) */
/**************************************************************************************************************************/
/* CDF */
NumericalScalar DistFunc::pBeta(const NumericalScalar p1,
                                const NumericalScalar p2,
                                const NumericalScalar x,
                                const Bool tail)
{
  const NumericalScalar value(SpecFunc::BetaRatioInc(p1, p2, x));
  return (tail ? 1.0 - value : value);
}
/* CDF Inverse */
NumericalScalar DistFunc::qBeta(const NumericalScalar p1,
                                const NumericalScalar p2,
                                const NumericalScalar q,
                                const Bool tail)
{
  if (tail)
    return SpecFunc::BetaRatioIncInv(p1, p2, 1.0 - q);
  else
    return SpecFunc::BetaRatioIncInv(p1, p2, q);
}
/* Random number generation
   We use the algorithm of Cheng (1978), Johnk, Atkinson and Whittaker (1979) 1 & 2 described in:
   Luc Devroye, "Non-Uniform RandomVariate Generation", Springer-Verlag, 1986, available online at:
   http://cg.scs.carleton.ca/~luc/nonuniformrandomvariates.zip
   and with the important errata at:
   http://cg.scs.carleton.ca/~luc/errors.pdf
*/
NumericalScalar DistFunc::rBeta(const NumericalScalar p1,
                                const NumericalScalar p2)
{
  // Strategy:
  // If (a = 1 and b = 1), Beta(1,1) = Uniform(0,1)
  // If (a = 1 or b = 1), analytic cases
  // If (a + b <= 1), Johnk
  // If (a + b > 1):
  // If (a < 1 and b < 1), Atkinson and Whittaker 1
  // If (a < 1 and b > 1) or (a > 1 and b < 1), Atkinson and Whittaker 2
  // If (a > 1 and b > 1) Cheng.
  if ((p1 == 1.0) && (p2 == 1.0)) return RandomGenerator::Generate();
  // Special case for p1 = 1 or p2 = 1
  if (p1 == 1.0)
    {
      return 1.0 - pow(RandomGenerator::Generate(), 1.0 / p2);
    }
  if (p2 == 1.0)
    {
      return pow(RandomGenerator::Generate(), 1.0 / p1);
    }
  // Now, the more general cases
  NumericalScalar minp(std::min(p1, p2));
  NumericalScalar maxp(std::max(p1, p2));
  NumericalScalar sum(p1 + p2);
  if (sum <= 1.0) // Johnk
    {
      // Use of logarithms to avoid underflow if minp << 1 (here 1e-3, quite arbitrary). It is the
      // only case where this kind of trick is useful. It should not be much slower than the usual
      // version using pow()
      if (minp < 1e-3)
        {
          for (;;)
            {
              NumericalScalar u(RandomGenerator::Generate());
              NumericalScalar v(RandomGenerator::Generate());
              NumericalScalar logx(log(u) / p1);
              NumericalScalar logy(log(v) / p2);
              NumericalScalar logsum((logx > logy) ? logx + log1p(exp(logy - logx)) : logy + log1p(exp(logx - logy)));
              // Acceptation step
              if (logsum <= 0.0) return exp(logy - logsum);
            }
        }
      // Usual form of the algorithm
      for (;;)
        {
          NumericalScalar u(RandomGenerator::Generate());
          NumericalScalar v(RandomGenerator::Generate());
          NumericalScalar x(pow(u, 1.0 / p1));
          NumericalScalar y(pow(v, 1.0 / p2));
          // Acceptation step
          if (x + y <= 1.0) return x / (x + y);
        }
    } // End Johnk
  // Now, sum > 1 for all the remaining cases
  if (minp > 1.0) // Cheng
    {
      NumericalScalar lambda(sqrt((sum - 2.0) / (2.0 * p1 * p2 - sum)));
      NumericalScalar c(minp + 1.0 / lambda);
      for (;;)
        {
          NumericalScalar u1(RandomGenerator::Generate());
          NumericalScalar u2(RandomGenerator::Generate());
          NumericalScalar v(lambda * log(u1 / (1.0 - u1)));
          NumericalScalar w(minp * exp(v));
          NumericalScalar z(u1 * u1 * u2);
          // 1.386294361119890618834464 = log(4)
          NumericalScalar r(c * v - 1.386294361119890618834464);
          NumericalScalar s(minp + r - w);
          // Quick acceptance steps
          // 2.609437912434100374600759 = 1 + log(5)
          if (s + 2.609437912434100374600759 >= 5.0 * z) return (p1 == minp) ? w / (maxp + w) : maxp / (maxp + w);
          NumericalScalar t(log(z));
          if (s > t) return (p1 == minp) ? w / (maxp + w) : maxp / (maxp + w);
          // Acceptance step
          if (r + sum * log(sum / (maxp + w)) >= t) return (p1 == minp) ? w / (maxp + w) : maxp / (maxp + w);
        }
    } // End Cheng
  if (maxp < 1.0) // Atkinson and Whittaker 1
    {
      NumericalScalar t(1.0 / (1.0 + sqrt(maxp * (1.0 - maxp) / (minp * (1.0 - minp)))));
      NumericalScalar tc(1.0 - t);
      NumericalScalar p(maxp * t / (maxp * t + minp * tc));
      for (;;)
        {
          NumericalScalar u(RandomGenerator::Generate());
          NumericalScalar e(-log(RandomGenerator::Generate()));
          if (u <= p)
            {
              NumericalScalar x(t * pow(u / p, 1.0 / minp));
              // Acceptation test
              if (e >= (1.0 - maxp) * log((1.0 - x) / tc)) return ((p1 == minp) ? x : 1.0 - x);
            }
          else
            {
              NumericalScalar x(1.0 - tc * pow((1.0 - u) / (1.0 - p), 1.0 / maxp));
              // Acceptation test
              if (e >= (1.0 - minp) * log(x / t)) return ((p1 == minp) ? x : 1.0 - x);
            }
        }
    } // End Atkinson and Whittaker 1
  // Remaining case, Atkinson and Whittaker 2
  NumericalScalar t((minp > 1) ? (1.0 - minp) / (maxp + 1.0 - minp) : 0.5);
  NumericalScalar tc(1.0 - t);
  NumericalScalar p(maxp * t / (maxp * t + minp * pow(tc, maxp)));
  for (;;)
    {
      NumericalScalar u(RandomGenerator::Generate());
      NumericalScalar e(-log(RandomGenerator::Generate()));
      if (u <= p)
        {
          NumericalScalar x(t * pow(u / p, 1.0 / minp));
          if (e >= (1.0 - maxp) * log1p(-x)) return ((p1 == minp) ? x : 1.0 - x);
        }
      else
        {
          NumericalScalar x(1.0 - tc * pow((1.0 - u) / (1.0 - p), 1.0 / maxp));
          if (e >= (1.0 - minp) * log(x / t)) return ((p1 == minp) ? x : 1.0 - x);
        }
    } // End Atkinson and Whittaker 2
} // End of rBeta

/*******************************************************************************************************/
/* Binomial distribution, i.e. with a PDF equals to C(n, p) p^k (1 - p)^(n - k) */
/*******************************************************************************************************/
/* Random number generation
   We use the rejection algorithm described in:
   Wolfgang Hormann, "The Generation of Binomial Random Variates",
   Journal of Statistical Computation and Simulation 46, pp. 101-110, 1993
   http://epub.wu.ac.at/1242/
*/
NumericalScalar DistFunc::fcBinomial(const UnsignedLong k)
{
  switch (k)
    {
    case 0:
      return 0.08106146679532726;
      break;
    case 1:
      return 0.04134069595540929;
      break;
    case 2:
      return 0.02767792568499834;
      break;
    case 3:
      return 0.02079067210376509;
      break;
    case 4:
      return 0.01664469118982119;
      break;
    case 5:
      return 0.01387612882307075;
      break;
    case 6:
      return 0.01189670994589177;
      break;
    case 7:
      return 0.01041126526197209;
      break;
    case 8:
      return 0.009255462182712733;
      break;
    case 9:
      return 0.008330563433362871;
      break;
    default:
      NumericalScalar kp1Sq(pow(k + 1, 2));
      return (1.0 / 12.0 - (1.0 / 360.0 - 1.0 / 1260. / kp1Sq) / kp1Sq) / (k + 1);
      break;
    } // switch
}

UnsignedLong DistFunc::rBinomial(const UnsignedLong n,
                                 const NumericalScalar p)
{
  // Quick return for degenerate cases
  if (p == 0.0) return 0;
  if (p == 1.0) return n;
  // Use symmetry
  NumericalScalar q(p);
  Bool complementary(q > 0.5);
  if (complementary) q = 1.0 - q;
  if (q == 1.0) return (complementary ? 0 : n);
  if (q == 0.0) return (complementary ? n : 0);
  // Small case, use inversion
  if (n * q <= 15)
    {
      const NumericalScalar r(q / (1.0 - q));
      NumericalScalar t(pow(1.0 - q, n));
      NumericalScalar s(t);
      const NumericalScalar u(RandomGenerator::Generate());
      for (UnsignedLong k = 0; k <= n; ++k)
        {
          if (s >= u) return (complementary ? n - k : k);
          t *= r * (n - k) / (k + 1.0);
          s += t;
        }
      // Should never go there, except in case of round-off errors
      return 0;
    }
  // Large case, use the algorithm described in the reference.
  // Setup
  const NumericalScalar m(floor((n + 1) * q));
  const NumericalScalar r(q / (1.0 - q));
  const NumericalScalar nr((n + 1) * r);
  const NumericalScalar npq(n * q * (1.0 - q));
  const NumericalScalar npqSqrt(sqrt(npq));
  const NumericalScalar b(1.15 + 2.53 * npqSqrt);
  const NumericalScalar a(-0.0873 + 0.0248 * b + 0.01 * q);
  const NumericalScalar c(n * q + 0.5);
  const NumericalScalar alpha((2.83 + 5.1 / b) * npqSqrt);
  const NumericalScalar vr(0.92 - 4.2 / b);
  const NumericalScalar urvr(0.86 * vr);
  NumericalScalar u(0.0);
  NumericalScalar k(0.0);
  // Main loop
  for (;;)
    {
      NumericalScalar v(RandomGenerator::Generate());
      if (v <= urvr)
        {
          u = v / vr - 0.43;
          k = floor((2 * a / (0.5 - fabs(u)) + b) * u + c);
          return (complementary ? static_cast<UnsignedLong>(n - k) : static_cast<UnsignedLong>(k));
        } // v <= urvr
      if (v >= vr)
        {
          u = RandomGenerator::Generate() - 0.5;
        } // v >= vr
      else
        {
          u = v / vr - 0.93;
          u = (u < 0.0 ? -0.5 : 0.5) - u;
          v = RandomGenerator::Generate() * vr;
        } // v < vr
      const NumericalScalar us(0.5 - fabs(u));
      k = floor((2.0 * a / us + b) * u + c);
      if ((k < 0.0) || (k > n)) continue;
      v = v * alpha / (a / (us * us) + b);
      const NumericalScalar km(fabs(k - m));
      // Recursive evaluation of f(k)
      if (km <= 15)
        {
          NumericalScalar f(1.0);
          if (m < k)
            {
              NumericalScalar i(m);
              do
                {
                  ++i;
                  f *= (nr / i - r);
                }
              while (i < k);
            } // m < k
          else if (m > k)
            {
              NumericalScalar i(k);
              do
                {
                  ++i;
                  v *= (nr / i - r);
                }
              while (i < m);
            } // m > k
          if (v <= f) return (complementary ? static_cast<UnsignedLong>(n - k) : static_cast<UnsignedLong>(k));
          continue;
        } // km <= 15
      // Squeeze-acceptance or rejection
      v = log(v);
      const NumericalScalar rho(km / npq * (((km / 3.0 + 0.625) * km + 1.0 / 6.0) / npq + 0.5));
      const NumericalScalar t(-km * km / (2.0 * npq));
      if (v < t - rho) return (complementary ? static_cast<UnsignedLong>(n - k) : static_cast<UnsignedLong>(k));
      if (v > t + rho) continue;
      const NumericalScalar nm(n - m + 1);
      const NumericalScalar h((m + 0.5) * log((m + 1) / (r * nm)) + fcBinomial(static_cast<UnsignedLong>(m)) + fcBinomial(static_cast<UnsignedLong>(n - m)));
      // Final acceptance-rejection
      const NumericalScalar nk(n - k + 1);
      if (v <= h + (n + 1) * log(nm / nk) + (k + 0.5) * log(nk * r / (k + 1)) - fcBinomial(static_cast<UnsignedLong>(k)) - fcBinomial(static_cast<UnsignedLong>(n - k))) return (complementary ? static_cast<UnsignedLong>(n - k) : static_cast<UnsignedLong>(k));
    } // for(;;)
} // rBinomial

/*******************************************************************************************************/
/* Normalized Gamma distribution, i.e. with a PDF equals to x ^ (k - 1) . exp(-x) / gamma(k) . (x > 0) */
/*******************************************************************************************************/
/* CDF */
NumericalScalar DistFunc::pGamma(const NumericalScalar k,
                                 const NumericalScalar x,
                                 const Bool tail)
{
#ifdef USE_DCDFLIB
  NumericalScalar ans;
  NumericalScalar qans;
  int ind(0);
  DCDFLIB::gamma_inc((double*)&k, (double*)&x, (double*)&ans, (double*)&qans, &ind);
  return (tail ? qans : ans);
#else
  const NumericalScalar value(SpecFunc::GammaInc(k, x) / SpecFunc::Gamma(k));
  return (tail ? 1.0 - value : value);
#endif
}
/* CDF Inverse */
NumericalScalar DistFunc::qGamma(const NumericalScalar k,
                                 const NumericalScalar q,
                                 const Bool tail)
{
#ifdef USE_DCDFLIB
  NumericalScalar X;
  NumericalScalar X0(0.0);
  NumericalScalar P(q);
  NumericalScalar Q(1.0 - q);
  if (tail)
    {
      const NumericalScalar swap(P);
      P = Q;
      Q = swap;
    }
  int ierr;
  DCDFLIB::gamma_inc_inv((double*)&k, (double*)&X, (double*)&X0, (double*)&P, (double*)&Q, &ierr);
  return X;
#else
  if (tail)
    return SpecFunc::GammaIncInv(k, (1.0 - q) * SpecFunc::Gamma(k));
  else
    return SpecFunc::GammaIncInv(k, q * SpecFunc::Gamma(k));
#endif
}
/* Random number generation
   We use the algorithm described in:
   George Marsaglia and Wai Wan Tsang, "A Simple Method for Generating Gamma
   Variables": ACM Transactions on Mathematical Software, Vol. 26, No. 3,
   September 2000, Pages 363-372
   with a small optimization on the beta that appears in the squeezing function (1 + beta * x^4)*exp(-x^2/2).
   We also add the special treatment of the case k < 1
*/
NumericalScalar DistFunc::rGamma(const NumericalScalar k)
{
  // Special case k < 1.0
  NumericalScalar correction(1.0);
  NumericalScalar alpha(k);
  if (alpha < 1.0)
    {
      correction = pow(RandomGenerator::Generate(), 1.0 / alpha);
      alpha++;
    }
  NumericalScalar d(alpha - 0.3333333333333333333333333);
  NumericalScalar c(1.0 / sqrt(9.0 * d));
  NumericalScalar x;
  NumericalScalar v;
  for (;;)
    {
      do
        {
          x = rNormal();
          v = 1.0 + c * x;
        }
      while (v <= 0.0);
      v = v * v * v;
      NumericalScalar u(RandomGenerator::Generate());
      NumericalScalar x2(x * x);
      // Quick acceptation test
      // 0.03431688782875261396035499 is the numerical solution of the squeezing
      // problem described in
      if (u < 1.0 - 0.03431688782875261396035499 * x2 * x2) return correction * d * v;
      // Acceptation test
      if (log(u) < 0.5 * x2 + d * (1.0 - v + log(v))) return correction * d * v;
    }
} // End of rGamma

/****************************/
/* Kolmogorov distribution. */
/****************************/
/* CDF
   Adapted from:
   George Marsaglia and Wai Wan Tsang and Jingbo Wang (2003),
   "Evaluating Kolmogorov's distribution".
   Journal of Statistical Software, Volume 8, 2003, Issue 18.
   URL: http://www.jstatsoft.org/v08/i18/.
*/
NumericalScalar DistFunc::pKolmogorov(const UnsignedLong n,
                                      const NumericalScalar x,
                                      const Bool tail)
{
  // If x <= 0, we are at the left of the support
  if (x <= 0.0) return (tail ? 1.0 : 0.0);
  // If x >= 1, we are at the right of the support
  if (x >= 1.0) return (tail ? 0.0 : 1.0);
  const NumericalScalar s(x * x * n);
  // Less accurate (7 digits only) but much faster in the right tail
  if (s > 7.14 || (s > 3.76 && n > 99))
    {
      // Here, result is the complementary CDF
      const NumericalScalar result(std::min(1.0, round(1.0e7 * (2 * exp(-(2.000071 + 0.331 / sqrt(n) + 1.409 / n) * s))) * 1.0e-7));
      // Here, result is the complementary CDF (one more time!)
      return (tail ? result : 1.0 - result);
    }
  // Now, the exact computation
  const UnsignedLong k(static_cast<UnsignedLong>(n * x + 1));
  const UnsignedLong m(2 * k - 1);
  SquareMatrix H(m);
  // Regular part of H
  const NumericalScalar factor(exp(SpecFunc::LnGamma(n + 1.0) / n - log(n)));
  for (UnsignedLong j = 0; j < m; ++j)
    {
      UnsignedLong k(1);
      NumericalScalar hij(factor);
      for (UnsignedLong i = j; i < m; ++i)
        {
          hij /= k;
          H(i, j) = hij;
          ++k;
        }
    }
  // Borders and upper diagonal of H
  const NumericalScalar h(k - n * x);
  NumericalScalar hi(h);
  for (UnsignedLong i = 1; i < m; ++i)
    {
      // Upper diagonal
      H(i - 1, i) = factor;
      // Left border
      H(i - 1, 0) *= (1.0 - hi);
      // Bottom border
      H(m - 1, m - i) *= (1.0 - hi);
      hi *= h;
    }
  // Left bottom corner, now hi = h^m
  H(m - 1, 0) *= (h > 0.5 ? 1.0 - 2.0 * hi + pow(2.0 * h - 1.0, m) : 1.0 - 2.0 * hi);
  // Here, result is the CDF
  const NumericalScalar result(H.power(n).operator()(k - 1, k - 1));
  return (tail ? 1.0 - result : result);
}

NumericalScalar DistFunc::pKolmogorovAsymptotic(const NumericalScalar x,
                                                const Bool tail)
{
  // If x <= 0, we are at the left of the support
  // In fact, in double precision the numerical range starts slightly above 0.04
  if (x <= 0.04) return (tail ? 0.0 : 1.0);
  // Small x case
  if (x < 1.0)
    {
      NumericalScalar sum(0.0);
      const NumericalScalar z(-M_PI * M_PI / (8.0 * x * x));
      const NumericalScalar w(log(x));
      // All the terms starting at kmax are negligible in double precision
      const UnsignedLong kmax(static_cast<UnsignedLong>(0.5 * (1.0 + sqrt(1.0 + log(ResourceMap::GetAsNumericalScalar("DistFunc-Precision")) / z)) + 1.0));
      for (UnsignedLong k = 1; k < kmax; k += 2)
        {
          sum += exp(k * k * z - w);
        }
      return (tail ? 1.0 - sum * sqrt(2.0 * M_PI) : sum * sqrt(2.0 * M_PI));
    }
  else
    {
      const NumericalScalar z(-2.0 * x * x);
      NumericalScalar s(1.0);
      UnsignedLong k(1);
      NumericalScalar oldValue(1.0);
      NumericalScalar newValue(0.0);
      const NumericalScalar precision(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"));
      while(fabs(oldValue - newValue) > precision)
        {
          oldValue = newValue;
          newValue += 2.0 * s * exp(z * k * k);
          s *= -1.0;
          ++k;
        }
      return (tail ? newValue : 1.0 - newValue);
    }
}

/***************************************************************************************************************/
/* Normalized NonCentralChiSquare distribution, i.e. with a PDF equals to (eq. 31.15 p.516 of the reference):  */
/* exp(-delta^2 / 2) * (nu / (nu + x^2)) ^ ((nu + 1) / 2) / (sqrt(nu * Pi) * Gamma(nu / 2)) * SUM              */
/* where SUM = sum_0^inf Gamma((nu + k + 1) / 2) * omega^k / Gamma(k + 1)                                      */
/* and omega = x * delta * sqrt(2 / (nu + x^2))                                                                */
/* In order to derive simple update formulas for the terms in the sum, we separate the odd indices from the    */
/* even ones:                                                                                                  */
/* SUM = SUM_ODD + SUM_EVEN, where:                                                                            */
/* SUM_ODD  = sum_0^inf Gamma(nu / 2 + k + 1) * omega * z^k / Gamma(2 * k + 2)                                 */
/* SUM_EVEN = sum_O^inf Gamma((nu + 1) / 2 + k) * z^k / Gamma(2 * k + 1)                                       */
/* and z = omega^2                                                                                             */
/* The summation is done starting at the kM chosen as for the pNonCentralStudent method. See                   */
/* Denise Benton, K. Krishnamoorthy, "Computing discrete mixtures of continuous distributions: noncentral      */
/* chisquare, noncentral t and the distribution of the square of the sample multiple correlation coefficient", */
/* Computational Statistics & Data Analysis, 43 (2003) pp 249-267                                              */
/* Reference:                                                                                                  */
/* Norman L. Johnson, Samuel Kotz, N. Balakrishnan, "Continuous univariate distributions volume 2", second     */
/* edition, 1995, Wiley Inter-Science                                                                          */
/***************************************************************************************************************/
/* PDF */
NumericalScalar DistFunc::dNonCentralChiSquare(const NumericalScalar nu,
                                               const NumericalScalar lambda,
                                               const NumericalScalar x)
{
  if (nu < 0.0) throw InvalidArgumentException(HERE) << "Error: the number of degrees of freedom nu must be >= 0.";
  if (lambda < 0.0) throw InvalidArgumentException(HERE) << "Error: the non-centrality parameter lambda must be >= 0.";
  if (x <= 0.0) return 0.0;
  const NumericalScalar halfNu(0.5 * nu);
  // Early exit for lambda == 0, central ChiSquare PDF
  const NumericalScalar precision(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"));
  if (fabs(lambda) < precision) return exp((halfNu - 1.0) * log(x) - 0.5 * x - SpecFunc::LnGamma(halfNu) - halfNu * M_LN2);
  // Case lambda <> 0
  const NumericalScalar halfLambda(0.5 * lambda);
  // Starting index in the sum: integral part of halfDelta2 and insure that it is at least 1
  const UnsignedLong k(std::max(static_cast<UnsignedLong>(1), static_cast<UnsignedLong>(floor(halfLambda))));
  // Loop forward and backward starting from k
  // Initialization
  NumericalScalar pForward(exp(-halfLambda - 0.5 * x + (halfNu + k - 1.0) * log(x) - SpecFunc::LogGamma(k + 1.0) - SpecFunc::LogGamma(halfNu + k) - (2.0 * k + halfNu) * M_LN2 + k * log(lambda)));
  NumericalScalar pBackward(pForward);
  NumericalScalar value(pForward);
  NumericalScalar error(SpecFunc::MaxNumericalScalar);
  UnsignedLong kForward(k);
  UnsignedLong kBackward(k);
#define FORWARD_ITERATION                                               \
  pForward *= halfLambda * x / (2.0 * (halfNu + kForward) * (kForward + 1)); \
  value += pForward;
#define BACKWARD_ITERATION                                              \
  pBackward *= 2.0 * (halfNu + kBackward - 1) * kBackward / (halfLambda * x); \
  value += pBackward;

  // Here, i is an UnsignedLong as it is only a loop counter
  UnsignedLong i(1);
  const UnsignedLong maximumIteration(ResourceMap::GetAsUnsignedLong("DistFunc-MaximumIteration"));
  const UnsignedLong imax(std::min(k, maximumIteration));
  //        while((error > precision * (fabs(value) + precision)) && (i <= imax))
  while((error > 0.0) && (i <= imax))
    {
      FORWARD_ITERATION;
      BACKWARD_ITERATION;
      error = pForward + pBackward;
      ++kForward;
      --kBackward;
      ++i;
    }
  // Do we have to perform further forward iterations?
  //        while ((error > precision * (fabs(value) + precision)) && (i <= MaximumIteration))
  while ((error > 0.0) && (i <= maximumIteration))
    {
      FORWARD_ITERATION;
      error = pForward;
      ++kForward;
      ++i;
    }
#undef FORWARD_ITERATION
#undef BACKWARD_ITERATION
  if (error > precision * (fabs(value) + precision)) LOGWARN(OSS() << "Warning: in DistFunc::dNonCentralChiSquare(nu, lambda, x), no convergence after " << i << " iterations. Error is " << error * value << " value is " << value << " for nu=" << nu << ", lambda=" << lambda << " and x=" << x);
  // Clip to [0,+inf[ in order to get rid of small rounding error
  return (value <= 0.0 ? 0.0 : value);
}
/* CDF
   We use the algorithm described in:
   Denise Benton, K. Krishnamoorthy, "Computing discrete mixtures of continuous distributions: noncentral chisquare, noncentral t
   and the distribution of the square of the sample multiple correlation coefficient",
   Computational Statistics & Data Analysis, 43 (2003) pp 249-267
*/
NumericalScalar DistFunc::pNonCentralChiSquare(const NumericalScalar nu,
                                               const NumericalScalar lambda,
                                               const NumericalScalar x,
                                               const Bool tail)
{
  if (nu < 0.0) throw InvalidArgumentException(HERE) << "Error: the number of degrees of freedom nu must be >= 0.";
  if (lambda < 0.0) throw InvalidArgumentException(HERE) << "Error: the non-centrality parameter lambda must be >= 0.";
  if (x <= 0.0) return (tail ? 1.0 : 0.0);
  const NumericalScalar halfNu(0.5 * nu);
  const NumericalScalar halfX(0.5 * x);
  // Early exit for lambda == 0, central ChiSquare PDF
  const NumericalScalar precision(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"));
  if (fabs(lambda) < precision) return pGamma(halfNu, halfX, tail);
  // Case lambda <> 0
  const NumericalScalar halfLambda(0.5 * lambda);
  // Starting index in the sum: integral part of halfDelta2 and insure that it is at least 1
  const UnsignedLong k(std::max(static_cast<UnsignedLong>(1), static_cast<UnsignedLong>(floor(halfLambda))));
  // Loop forward and backward starting from k
  // Initialization
  const NumericalScalar logHalfX(log(halfX));
  NumericalScalar xForward(exp((halfNu + k - 1) * logHalfX - halfX - SpecFunc::LogGamma(halfNu + k)));
  NumericalScalar expForward(exp(-halfLambda + k * log(halfLambda) - SpecFunc::LogGamma(k + 1.0)));
  NumericalScalar gammaForward(pGamma(halfNu + k, halfX));
  NumericalScalar pForward(expForward * gammaForward);
  NumericalScalar xBackward(xForward);
  NumericalScalar expBackward(expForward);
  NumericalScalar gammaBackward(gammaForward);
  NumericalScalar pBackward(expBackward * gammaBackward);
  NumericalScalar value(pForward);
  NumericalScalar error(SpecFunc::MaxNumericalScalar);
  UnsignedLong kForward(k);
  UnsignedLong kBackward(k);
#define FORWARD_ITERATION                       \
  xForward *= halfX / (halfNu + kForward);      \
  expForward *= halfLambda / (kForward + 1);    \
  gammaForward -= xForward;                     \
  pForward = expForward * gammaForward;         \
  value += pForward;
#define BACKWARD_ITERATION                              \
  expBackward *= kBackward / halfLambda;                \
  gammaBackward += xBackward;                           \
  xBackward *= (halfNu + kBackward - 1) / halfX;        \
  pBackward = expBackward * gammaBackward;              \
  value += pBackward;

  // Here, i is an UnsignedLong as it is only a loop counter
  UnsignedLong i(1);
  const UnsignedLong maximumIteration(ResourceMap::GetAsUnsignedLong("DistFunc-MaximumIteration"));
  const UnsignedLong imax(std::min(k, maximumIteration));
  //        while((error > precision * (fabs(value) + precision)) && (i <= imax))
  while((error > 0.0) && (i <= imax))
    {
      FORWARD_ITERATION;
      BACKWARD_ITERATION;
      error = pForward + pBackward;
      ++kForward;
      --kBackward;
      ++i;
    }
  // Do we have to perform further forward iterations?
  //        while ((error > precision * (fabs(value) + precision)) && (i <= MaximumIteration))
  while ((error > 0.0) && (i <= maximumIteration))
    {
      FORWARD_ITERATION;
      error = pForward;
      ++kForward;
      ++i;
    }
#undef FORWARD_ITERATION
#undef BACKWARD_ITERATION
  if (error > precision * (fabs(value) + precision)) LOGWARN(OSS() << "Warning: in DistFunc::dNonCentralChiSquare(nu, lambda, x), no convergence after " << i << " iterations. Error is " << error * value << " value is " << value << " for nu=" << nu << ", lambda=" << lambda << " and x=" << x);
  // Clip to [0,1] in order to get rid of small rounding error
  value = (value < 0.0 ? 0.0 : (value > 1.0 ? 1.0 : value));
  // Check if we are asked for the tail CDF
  if (tail) value = 1.0 - value;
  return value;
}
/* Random number generation
   We use a transformation method based on Gamma and Normal transformation:
   If N is Normal(delta, 1) distributed and G is Gamma(nu / 2) distributed,
   sqrt(2 * nu) * N / sqrt(G) is distributed according to NonCentralChiSquare(nu, delta)
*/
NumericalScalar DistFunc::rNonCentralChiSquare(const NumericalScalar nu,
                                               const NumericalScalar lambda)
{
  if (nu < 0.0) throw InvalidArgumentException(HERE) << "Error: the number of degrees of freedom nu must be >= 0.";
  if (lambda < 0.0) throw InvalidArgumentException(HERE) << "Error: the non-centrality parameter lambda must be >= 0.";
  // If the non-central parameter is zero return a usual chi-square realization
  if (lambda == 0.0) return 2.0 * rGamma(0.5 * nu);
  // Use the decomposition of a zero degree of freedom non-central chisquare and a nu degrees of freedom central chisquare
  const NumericalScalar n(rPoisson(0.5 * lambda));
  return 2.0 *  rGamma(0.5 * nu + n);
  //         return 2.0 * (rGamma(0.5 * n) + rGamma(0.5 * nu));
}
/************************************************************************************************************/
/* Normalized NonCentralStudent distribution, i.e. with a PDF equals to (eq. 31.15 p.516 of the reference): */
/* exp(-delta^2 / 2) * (nu / (nu + x^2)) ^ ((nu + 1) / 2) / (sqrt(nu * Pi) * Gamma(nu / 2)) * SUM           */
/* where SUM = sum_0^inf Gamma((nu + k + 1) / 2) * omega^k / Gamma(k + 1)                                   */
/* and omega = x * delta * sqrt(2 / (nu + x^2))                                                             */
/* In order to derive simple update formulas for the terms in the sum, we separate the odd indices from the */
/* even ones:                                                                                               */
/* SUM = SUM_ODD + SUM_EVEN, where:                                                                         */
/* SUM_ODD  = sum_0^inf Gamma(nu / 2 + k + 1) * omega * z^k / Gamma(2 * k + 2)                              */
/* SUM_EVEN = sum_O^inf Gamma((nu + 1) / 2 + k) * z^k / Gamma(2 * k + 1)                                    */
/* and z = omega^2                                                                                          */
/* The summation is done starting at the kM chosen as for the pNonCentralStudent method.                    */
/* Reference:                                                                                               */
/* Norman L. Johnson, Samuel Kotz, N. Balakrishnan, "Continuous univariate distributions volume 2", second  */
/* edition, 1995, Wiley Inter-Science                                                                       */
/************************************************************************************************************/
/* PDF */
NumericalScalar DistFunc::dNonCentralStudent(const NumericalScalar nu,
                                             const NumericalScalar delta,
                                             const NumericalScalar x)
{
  // Early exit for delta == 0, central Student PDF
  const NumericalScalar precision(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"));
  if (fabs(delta) < precision) return exp(SpecFunc::LnGamma(0.5 * nu + 0.5) - SpecFunc::LnGamma(0.5 * nu) - 0.5 * log(M_PI * nu) + (0.5 * nu + 0.5) * log(nu / (nu + x * x)));
  // Case delta <> 0
  const NumericalScalar halfNu(0.5 * nu);
  const NumericalScalar halfNup1_2(halfNu + 0.5);
  const NumericalScalar logConstant(-0.5 * delta * delta - SpecFunc::LnGamma(halfNu) - 0.5 * log(M_PI * nu));
  // Early exit for x == 0
  if (fabs(x) < precision) return exp(logConstant + SpecFunc::LnGamma(halfNup1_2));
  // For x <> 0
  const NumericalScalar x2(x * x);
  const NumericalScalar w(1.0 / (nu + x2));
  NumericalScalar logFactor(logConstant + halfNup1_2 * log(nu * w));
  // Special treatment for very low value to avoid NaNs due to 0.Inf
  static const NumericalScalar logPrecision(log(precision));
  if (logFactor < logPrecision)
    {
      NumericalScalar value(0.0);
      if (x < 0.0) value = nu / x * (pNonCentralStudent(nu + 2.0, delta, x * sqrt(1.0 + 2.0 / nu)) - pNonCentralStudent(nu, delta, x));
      else value = -nu / x * (pNonCentralStudent(nu + 2.0, -delta, -x * sqrt(1.0 + 2.0 / nu)) - pNonCentralStudent(nu, -delta, -x));
      return (value <= 0.0 ? 0.0 : value);
    }

  const NumericalScalar omega(delta * x * sqrt(2 * w));
  const NumericalScalar z(omega * omega);
  // Start at even index that maximize the coefficient in the sum
  const NumericalScalar halfDelta2(0.5 * delta * delta);
  // Starting index in the sum: integral part of halfDelta2 and insure that it is at least 1
  const UnsignedLong k(std::max(static_cast<UnsignedLong>(1), static_cast<UnsignedLong>(floor(halfDelta2))));
  // Loop forward and backward starting from k
  // Initialization
  const NumericalScalar kLogZ(k * log(z));
  NumericalScalar pForwardEven(exp(logFactor + SpecFunc::LnGamma(halfNup1_2 + k) - SpecFunc::LnGamma(2 * k + 1) + kLogZ));
  NumericalScalar pForwardOdd(omega * exp(logFactor + SpecFunc::LnGamma(halfNu + k + 1) - SpecFunc::LnGamma(2 * k + 2) + kLogZ));
  NumericalScalar pBackwardEven(pForwardEven);
  NumericalScalar pBackwardOdd(pForwardOdd);
  NumericalScalar value(pForwardOdd + pForwardEven);
  NumericalScalar error(SpecFunc::MaxNumericalScalar);
  UnsignedLong kForward(k);
  UnsignedLong kBackward(k);
#define FORWARD_ITERATION                                               \
  pForwardOdd *= (halfNu + kForward + 1) * z / (2 * (kForward + 1) * (2 * kForward + 3)); \
  pForwardEven *= (halfNup1_2 + kForward) * z / (2 * (kForward + 1) * (2 * kForward + 1)); \
  value += pForwardOdd + pForwardEven;
#define BACKWARD_ITERATION                                              \
  pBackwardOdd *= 2 * kBackward * (2 * kBackward + 1) / (z * (halfNu + kBackward)); \
  pBackwardEven *= 2 * kBackward * (2 * kBackward - 1) / (z * (halfNup1_2 + kBackward - 1)); \
  value += pBackwardOdd + pBackwardEven;

  // Here, i is an UnsignedLong as it is only a loop counter
  UnsignedLong i(1);
  const UnsignedLong maximumIteration(ResourceMap::GetAsUnsignedLong("DistFunc-MaximumIteration"));
  const UnsignedLong imax(std::min(k, maximumIteration));
  //        while((error > precision * (fabs(value) + precision)) && (i <= imax))
  while((error > 0.0) && (i <= imax))
    {
      FORWARD_ITERATION;
      BACKWARD_ITERATION;
      error = pForwardOdd + pBackwardOdd + pForwardEven + pBackwardEven;
      ++kForward;
      --kBackward;
      ++i;
    }
  // Do we have to perform further forward iterations?
  //        while ((error > precision * (fabs(value) + precision)) && (i <= MaximumIteration))
  while ((error > 0.0) && (i <= maximumIteration))
    {
      FORWARD_ITERATION;
      error = pForwardOdd + pForwardEven;
      ++kForward;
      ++i;
    }
#undef FORWARD_ITERATION
#undef BACKWARD_ITERATION
  if (error > precision * (fabs(value) + precision)) LOGWARN(OSS() << "Warning: in DistFunc::dNonCentralStudent(nu, delta, x), no convergence after " << i << " iterations. Error is " << error * value << " value is " << value << " for nu=" << nu << ", delta=" << delta << " and x=" << x);
  // Clip to [0,+inf[ in order to get rid of small rounding error
  return (value <= 0.0 ? 0.0 : value);
}
/* CDF
   We use the algorithm described in:
   Denise Benton, K. Krishnamoorthy, "Computing discrete mixtures of continuous distributions: noncentral chisquare, noncentral t
   and the distribution of the square of the sample multiple correlation coefficient",
   Computational Statistics & Data Analysis, 43 (2003) pp 249-267
*/
NumericalScalar DistFunc::pNonCentralStudent(const NumericalScalar nu,
                                             const NumericalScalar delta,
                                             const NumericalScalar x,
                                             const Bool tail)
{
  // Special case when |delta| << 1
  const NumericalScalar precision(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"));
  if (fabs(delta) < precision) return pStudent(nu, x, tail);
  // Special case when |x| << 1
  if (fabs(x) < precision) return pNormal(-delta, tail);

  NumericalScalar t(x);
  NumericalScalar del(delta);
  // Must use the complementary function for negative arguments
  if (x < 0.0)
    {
      t = -t;
      del = -del;
    }
  // Some useful quantities
  const NumericalScalar x2(t * t);
  const NumericalScalar xi(x2 / (nu + x2));
  const NumericalScalar logXi(log(xi));
  const NumericalScalar halfNu(0.5 * nu);
  const NumericalScalar halfDelta2(0.5 * del * del);
  const NumericalScalar logHalfDelta2(log(halfDelta2));
  // Starting index in the sum: integral part of halfDelta2 and insure that it is at least 1
  const UnsignedLong k(std::max(static_cast<UnsignedLong>(1), static_cast<UnsignedLong>(floor(halfDelta2))));
  // Index of the forward iterations
  UnsignedLong kForward(k);
  // Index of the backward iterations
  UnsignedLong kBackward(k);
  // Terms and factors of the summation.
  // The initialization corresponds to the terme of index k.
  const NumericalScalar commonExponent(-halfDelta2 + k * logHalfDelta2);
  NumericalScalar pForward(0.5 * exp(commonExponent - SpecFunc::LnGamma(k + 1)));
  NumericalScalar qForward(0.5 * del / M_SQRT2 * exp(commonExponent - SpecFunc::LnGamma(k + 1.5)));
  NumericalScalar betaPForward(pBeta(k + 0.5, halfNu, xi));
  NumericalScalar betaQForward(pBeta(k + 1, halfNu, xi));
  // The correction factors will be updated at the beginning of each iteration: it is the quantity to add to
  // the corresponding betaP and betaQ factors to go to the value associated with the current iteration.
  // They are thus initialized such that after one update, they will change the betaP and betaQ factors
  // to their values associated with the iteration (k-1):
  const NumericalScalar commonFactor((k - 0.5) * logXi + halfNu * log(nu / (nu + x2)) - SpecFunc::LnGamma(halfNu));
  // correctionBetaPForward = Gamma(k - 1/2 + nu/2) / Gamma(k + 1/2) / Gamma(nu/2) * xi^(k - 1/2) * (1 - xi)^(nu/2)
  NumericalScalar correctionBetaPForward(-exp(SpecFunc::LnGamma(k - 0.5 + halfNu) - SpecFunc::LnGamma(k + 0.5) + commonFactor));
  // correctionBetaPForward = Gamma(k + nu/2) / Gamma(k + 1) / Gamma(nu/2) * xi^k * (1 - xi)^(nu/2)
  NumericalScalar correctionBetaQForward(-exp(SpecFunc::LnGamma(k + halfNu) - SpecFunc::LnGamma(k + 1) + commonFactor + 0.5 * logXi));
  NumericalScalar pBackward(pForward);
  NumericalScalar qBackward(qForward);
  NumericalScalar betaPBackward(betaPForward);
  NumericalScalar betaQBackward(betaQForward);
  // The correction factors will be updated at the beginning of each iteration: it is the quantity to add to
  // the corresponding betaP and betaQ factors to go to the value associated with the current iteration.
  // They are thus initialized such that after one update, they will change the betaP and betaQ factors
  // to their values associated with the iteration (k+1):
  // correctionBetaPBackward = Gamma(k + 1/2 + nu/2) / Gamma(k + 3/2 + nu/2) / Gamma(nu/2) * xi^(k + 1/2) * (1 - xi)^(nu/2)
  NumericalScalar correctionBetaPBackward(-correctionBetaPForward * xi * (k - 0.5 + halfNu) / (k + 0.5));
  NumericalScalar correctionBetaQBackward(-correctionBetaQForward * xi * (k + halfNu) / (k + 1));
  NumericalScalar value(pNormal(-del) + pForward * betaPForward + qForward * betaQForward);
  NumericalScalar contributionForward(0.0);
  NumericalScalar contributionBackward(0.0);
  NumericalScalar error(SpecFunc::MaxNumericalScalar);
  // At the beginning of the iteration, kForward and kBackward store the index of the last terms
  // that have already been accumulated. Each iteration must update the P and Q factors, as well
  // as the betaP and betaQ factors. For this last update, one must update the corresponding
  // correction factors. Thus, we proceed as follows:
  // First, update the correction factors for the betaP and betaQ factors
  // Second, update the betaP and betaQ factors
  // Third, upate the P and Q factors
  // Fourth, accumulate the current contribution
  // It is the responsibility of the main iteration to update kForward and kBackward
#define FORWARD_ITERATION                                               \
  correctionBetaPForward *= xi * (kForward - 0.5 + halfNu) / (kForward + 0.5); \
  correctionBetaQForward *= xi * (kForward + halfNu) / (kForward + 1);  \
  pForward *= halfDelta2 / (kForward + 1);                              \
  betaPForward += correctionBetaPForward;                               \
  qForward *= halfDelta2 / (kForward + 1.5);                            \
  betaQForward += correctionBetaQForward;                               \
  contributionForward = pForward * betaPForward + qForward * betaQForward; \
  value += contributionForward;

#define BACKWARD_ITERATION                                              \
  correctionBetaPBackward *= (kBackward + 0.5) / (xi * (kBackward - 0.5 + halfNu)); \
  correctionBetaQBackward *= (kBackward + 1) / (xi * (kBackward + halfNu)); \
  pBackward *= kBackward / halfDelta2;                                  \
  betaPBackward += correctionBetaPBackward;                             \
  qBackward *= (kBackward + 0.5) / halfDelta2;                          \
  betaQBackward += correctionBetaQBackward;                             \
  contributionBackward = pBackward * betaPBackward + qBackward * betaQBackward; \
  value += contributionBackward;

  // Here, i is an UnsignedLong as it is only a loop counter
  UnsignedLong i(1);

  const UnsignedLong maximumIteration(ResourceMap::GetAsUnsignedLong("DistFunc-MaximumIteration"));
  const UnsignedLong imax(std::min(k, maximumIteration));
  while((error > 0.0) && (i <= imax))
    {
      FORWARD_ITERATION;
      BACKWARD_ITERATION;
      error = contributionForward + contributionBackward;
      ++kForward;
      --kBackward;
      ++i;
    }
  // Do we have to perform further forward iterations?
  while ((error > 0.0) && (i <= maximumIteration))
    {
      FORWARD_ITERATION;
      error = contributionForward;
      ++kForward;
      ++i;
    }
#undef FORWARD_ITERATION
#undef BACKWARD_ITERATION
  if (error > precision * (fabs(value) + precision)) LOGWARN(OSS() << "Warning: in DistFunc::pNonCentralStudent(nu, delta, x), no convergence after " << i << " iterations. Error is " << error * value << " value is " << value << " for nu=" << nu << ", delta=" << delta << " and x=" << x);
  // Clip to [0,1] in order to get rid of small rounding error
  value = (value < 0.0 ? 0.0 : (value > 1.0 ? 1.0 : value));
  // Check if we had to change the sign of the argument or if we are asked for the tail CDF
  if ((tail && (x > 0.0)) || (!tail && (x < 0.0))) value = 1.0 - value;
  return value;
}
/* Random number generation
   We use a transformation method based on Gamma and Normal transformation:
   If N is Normal(delta, 1) distributed and G is Gamma(nu / 2) distributed,
   sqrt(2 * nu) * N / sqrt(G) is distributed according to NonCentralStudent(nu, delta)
*/
NumericalScalar DistFunc::rNonCentralStudent(const NumericalScalar nu,
                                             const NumericalScalar delta)
{
  NumericalScalar n(rNormal() + delta);
  NumericalScalar g(rGamma(0.5 * nu));
  return sqrt(0.5 * nu / g) * n;
}
/**************************************************************************************/
/* Normalized Normal distribution, i.e. with a PDF equals to exp(-x^2/2) / sqrt(2.Pi) */
/**************************************************************************************/
/* CDF */
NumericalScalar DistFunc::pNormal(const NumericalScalar x,
                                  const Bool tail)
{
#ifdef USE_DCDFLIB
  NumericalScalar result;
  NumericalScalar ccum;
  DCDFLIB::cumnor((double*)&x, (double*)&result, (double*)&ccum);
  if (tail) return ccum;
  return result;
#else
  if (tail) return 0.5 - 0.5 * erf(x * M_SQRT1_2);
  return 0.5 + 0.5 * erf(x * M_SQRT1_2);
#endif
}

NumericalScalar DistFunc::pNormal2D(const NumericalScalar x1,
                                    const NumericalScalar x2,
                                    const NumericalScalar rho,
                                    const Bool tail,
                                    const Bool newAlgo)
{
  if (newAlgo) return Normal2DCDF(x1, x2, rho, tail);
  int zero(0);
  double r(rho);
  double u[2];
  u[0] = x1;
  u[1] = x2;
  return BVTL_F77(&zero, &(u[0]), &(u[1]), &r);
}

NumericalScalar DistFunc::pNormal3D(const NumericalScalar x1,
                                    const NumericalScalar x2,
                                    const NumericalScalar x3,
                                    const NumericalScalar rho12,
                                    const NumericalScalar rho13,
                                    const NumericalScalar rho23,
                                    const Bool tail,
                                    const Bool newAlgo)
{
  if (newAlgo) return Normal3DCDF(x1, x2, x3, rho12, rho13, rho23, tail);
  int zero(0);
  double r[3];
  r[0] = rho12;
  r[1] = rho13;
  r[2] = rho23;
  double u[3];
  u[0] = x1;
  u[1] = x2;
  u[2] = x3;
  double eps(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"));
  return TVTL_F77(&zero, &(u[0]), &r[0], &eps);
}

/* CDF inverse */
NumericalScalar DistFunc::qNormal(const NumericalScalar q,
                                  const Bool tail)
{
  if (tail)
    {
      if (q == 0.0) return 8.292303027957548;
      if (q == 1.0) return -31.0;
    }
  else
    {
      if (q == 1.0) return -31.0;
      if (q == 0.0) return 8.292303027957548;
    }
#ifdef USE_DCDFLIB
  NumericalScalar P(q);
  NumericalScalar Q(1.0 - q);
  const NumericalScalar result(DCDFLIB::dinvnr((double*)&P, (double*)&Q));
  if (tail) return -result;
  return result;
#else
  if (tail) return -M_SQRT2l * SpecFunc::ErfInv(2.0 * q - 1.0);
  return M_SQRT2l * SpecFunc::ErfInv(2.0 * q - 1.0);
#endif
}

/* Random number generation
   We use the improved ziggurat method, see:
   Doornik, J.A. (2005), "An Improved Ziggurat Method to Generate Normal
   Random Samples", mimeo, Nuffield College, University of Oxford,
   and www.doornik.com/research.
*/
NumericalScalar DistFunc::rNormal()
{
  for (;;)
    {
      NumericalScalar u(2.0 * RandomGenerator::Generate() - 1.0);
      UnsignedLong index(RandomGenerator::IntegerGenerate(DistFunc::NumberOfBandNormalZigurrat));
      /* Are we in a rectangular band of the ziggurat? */
      if (fabs(u) < DistFunc::NormalZigguratRatio[index])
        {
          return u * NormalZigguratAbscissa[index + 1];
        }
      /* No, we are either on a wedge or in the upper tail of the Normal distribution */
      /* Are we in the bottom band? Sample from the tail of the Normal distribution */
      if (index == DistFunc::NumberOfBandNormalZigurrat - 1)
        {
          NumericalScalar x;
          NumericalScalar y;
          /* Marsaglia method */
          do
            {
              x = log(RandomGenerator::Generate()) / DistFunc::NormalZigguratTail;
              y = log(RandomGenerator::Generate());
            }
          while (-(y + y) < x * x);
          return (u > 0.0) ? x - DistFunc::NormalZigguratTail : DistFunc::NormalZigguratTail - x;
        }
      /* Are we in the wedges? Basic rejection method */
      NumericalScalar xI(NormalZigguratAbscissa[index]);
      NumericalScalar xIp1(NormalZigguratAbscissa[index + 1]);
      NumericalScalar x(u * xIp1);
      NumericalScalar pdfX(  exp(-0.5 * x * x));
      NumericalScalar pdfI(  exp(-0.5 * xI * xI));
      NumericalScalar pdfIp1(exp(-0.5 * xIp1 * xIp1));
      if (RandomGenerator::Generate() * (pdfI - pdfIp1) < pdfX - pdfIp1)
        {
          return x;
        }
    }
}

/**********************************************************************************/
/* Poisson distribution, i.e. with a PDF equals to exp(-lambda) . lambda ^ k / k! */
/**********************************************************************************/
/* Random number generation
   For the small values of lambda, we use the method of inversion by sequential search described in:
   Luc Devroye, "Non-Uniform RandomVariate Generation", Springer-Verlag, 1986, available online at:
   http://cg.scs.carleton.ca/~luc/nonuniformrandomvariates.zip
   and with the important errata at:
   http://cg.scs.carleton.ca/~luc/errors.pdf
   For the large values of lambda, we use the ratio of uniform method described in:
   E. Stadlober, "The ratio of uniforms approach for generating discrete random variates". Journal of Computational and Applied Mathematics, vol. 31, no. 1, 1990, pp. 181-189.
*/
NumericalScalar DistFunc::rPoisson(const NumericalScalar lambda)
{
  NumericalScalar mu(floor(lambda));
  // Small case. The bound 6 is quite arbitrary, but must be < 80 to avoid overflow.
  if (mu < 6)
    {
      NumericalScalar x(0.0);
      NumericalScalar sum(exp(-lambda));
      NumericalScalar prod(sum);
      NumericalScalar u(RandomGenerator::Generate());
      for (;;)
        {
          if (u <= sum) return x;
          x++;
          prod *= lambda / x;
          sum += prod;
        }
    } // Small case
  // Large case
  NumericalScalar hatCenter(lambda + 0.5);
  NumericalScalar mode(floor(lambda));
  NumericalScalar logLambda(log(lambda));
  NumericalScalar pdfMode(mode * logLambda - SpecFunc::LnGamma(mode + 1.0));
  // 2.943035529371538572764190 = 8 / e
  // 0.898916162058898740826254 = 3 - 2 sqr(3 / e)
  NumericalScalar hatWidth(sqrt(2.943035529371538572764190 * (lambda + 0.5)) + 0.898916162058898740826254);
  NumericalScalar safetyBound(hatCenter + 6.0 * hatWidth);
  for (;;)
    {
      NumericalScalar u(RandomGenerator::Generate());
      NumericalScalar x(hatCenter + hatWidth * (RandomGenerator::Generate() - 0.5) / u);
      if (x < 0 || x >= safetyBound) continue;
      NumericalScalar k(floor(x));
      NumericalScalar logPdf(k * logLambda - SpecFunc::LnGamma(k + 1.0) - pdfMode);
      // Quick acceptance
      if (logPdf >= u * (4.0 - u) - 3.0) return k;
      // Quick rejection
      if (u * (u - logPdf) > 1.0) continue;
      // Acceptance
      if (2.0 * log(u) <= logPdf) return k;
    }
}

/********************************************************************************************************************************/
/* Normalized Student distribution, i.e. with a PDF equals to (1 + x^2 / nu)^(-(1 + nu) / 2) / (sqrt(nu) . Beta(1 / 2, nu / 2)) */
/********************************************************************************************************************************/
/* CDF */
NumericalScalar DistFunc::pStudent(const NumericalScalar nu,
                                   const NumericalScalar x,
                                   const Bool tail)
{
  if (nu == 1.0) return (tail ? 0.5 - atan(x) * M_1_PI : 0.5 + atan(x) * M_1_PI);
  if (nu == 2.0) return (tail ? 0.5 - x / sqrt(2.0 + x * x) : 0.5 + x / sqrt(2.0 + x * x));
#ifdef USE_DCDFLIB
  NumericalScalar cum;
  NumericalScalar ccum;
  DCDFLIB::cumt((double*)&x, (double*)&nu, (double*)&cum, (double*)&ccum);
  if (tail) return ccum;
  return cum;
#else
  const NumericalScalar value(SpecFunc::BetaInc(0.5 * nu, 0.5, x));
  return (tail ? 1.0 - value : value);
#endif
}
/* CDF inverse */
NumericalScalar DistFunc::qStudent(const NumericalScalar nu,
                                   const NumericalScalar q,
                                   const Bool tail)
{
  if (nu == 1.0)
    {
      if (tail)
        return tan((0.5 - q) * M_PI);
      else
        return tan((q - 0.5) * M_PI);
    }
#ifdef USE_DCDFLIB
  int which(2);
  NumericalScalar t;
  NumericalScalar P(q);
  NumericalScalar Q(1.0 - q);
  if (tail)
    {
      const NumericalScalar swap(P);
      P = Q;
      Q = swap;
    }
  int status;
  NumericalScalar bound;
  DCDFLIB::cdft(&which, (double*)&P, (double*)&Q, (double*)&t, (double*)&nu, &status, (double*)&bound);
  return t;
#else
  throw NotYetImplementedException(HERE);
#endif
}
/* Random number generation
   We use a transformation method based on Gamma and Normal transformation:
   If N is Normal(0, 1) distributed and G is Gamma(nu / 2) distributed,
   sqrt(2 * nu) * N / sqrt(G) is distributed according to Student(nu)
*/
NumericalScalar DistFunc::rStudent(const NumericalScalar nu)
{
  NumericalScalar n(rNormal());
  NumericalScalar g(rGamma(0.5 * nu));
  return sqrt(0.5 * nu / g) * n;
}

/* Compute the expectation of the min of n independent standard normal random variables. Usefull for the modified moment estimator of the LogNormal distribution. */
NumericalScalar DistFunc::eZ1(const UnsignedLong n)
{
  if (n <= 0) throw InvalidArgumentException(HERE) << "Error: n must be strictly positive.";
  static NumericalScalar nodes[128] = {9.8079096926749782026033390e-02, 2.9424096921218469797991561e-01, 4.9041387858718514746809250e-01, 6.8660518909321215660380258e-01, 8.8282227375329345793892660e-01, 1.0790725181825577535496122e+00, 1.2753633242167377696353796e+00, 1.4717021135638638145739759e+00, 1.6680963314844359561817086e+00, 1.8645534505054447383309383e+00, 2.0610809741737079558017920e+00, 2.2576864408541048712667240e+00, 2.4543774275784199432866622e+00, 2.6511615539506562785567740e+00, 2.8480464861148453552909053e+00, 3.0450399407915649134290777e+00, 3.2421496893895821993047513e+00, 3.4393835621992660247489466e+00, 3.6367494526746595077587933e+00, 3.8342553218113771844054655e+00, 4.0319092026277868364264334e+00, 4.2297192047572594302912378e+00, 4.4276935191596217359545187e+00, 4.6258404229603273857896694e+00, 4.8241682844262754351411023e+00, 5.0226855680876531919920886e+00, 5.2214008400156647181628365e+00, 5.4203227732665307412164732e+00, 5.6194601535027128016005275e+00, 5.8188218848029276413744931e+00, 6.0184169956731807988206196e+00, 6.2182546452717651529731537e+00, 6.4183441298619452136612962e+00, 6.6186948895068861733967826e+00, 6.8193165150222935201989526e+00, 7.0202187552032102976570365e+00, 7.2214115243424814429173638e+00, 7.4229049100595452662318140e+00, 7.6247091814594590441665212e+00, 7.8268347976434177117924059e+00, 8.0292924165934915265672000e+00, 8.2320929044559011583234267e+00, 8.4352473452488789318099250e+00, 8.6387670510230462242060088e+00, 8.8426635725042840919313109e+00, 9.0469487102513035219338200e+00, 9.2516345263625515942763530e+00, 9.4567333567697407325779683e+00, 9.6622578241581829095747019e+00, 9.8682208515572746537567071e+00, 1.0074635676647940494041886e+01, 1.0281515866837634049248891e+01, 1.0488875335157653191251042e+01, 1.0696728357042088900345860e+01, 1.0905089588052741961530773e+01, 1.1113974082619858662822321e+01, 1.1323397313874613884362823e+01, 1.1533375194655972686401645e+01, 1.1743924099781963641404176e+01, 1.1955060889683582572281695e+01, 1.2166802935508609351446676e+01, 1.2379168145812671465813240e+01, 1.2592174994966049877127273e+01, 1.2805842553417136659329113e+01, 1.3020190519967281793289524e+01, 1.3235239256227193956928362e+01, 1.3451009823442300506862860e+01, 1.3667524021893770835767368e+01, 1.3884804433103548556990800e+01, 1.4102874465096049669738234e+01, 1.4321758400996546516308293e+01, 1.4541481451277113257067713e+01, 1.4762069809995873134112522e+01, 1.4983550715414763026069010e+01, 1.5205952515425821882539839e+01, 1.5429304738266944762150950e+01, 1.5653638169066098815394345e+01, 1.5878984932819323859862773e+01, 1.6105378584483803337592983e+01, 1.6332854206954513949199624e+01, 1.6561448517793378857450989e+01, 1.6791199985695774874822891e+01, 1.7022148957813457866272315e+01, 1.7254337799208822083094216e+01, 1.7487811045896949980107817e+01, 1.7722615573144060013318108e+01, 1.7958800780939722006343882e+01, 1.8196418798852933084511311e+01, 1.8435524712827875258644471e+01, 1.8676176816885087724358203e+01, 1.8918436893181770856317469e+01, 1.9162370524468337507669120e+01, 1.9408047443678915527648282e+01, 1.9655541926238779717773660e+01, 1.9904933231696581852007900e+01, 2.0156306102538408960103184e+01, 2.0409751329571602386146602e+01, 2.0665366395153397356064835e+01, 2.0923256207880053036728538e+01, 2.1183533945274238266112570e+01, 2.1446322024681900006387514e+01, 2.1711753227242152331884139e+01, 2.1979972005732402979093282e+01, 2.2251136014735937033989485e+01, 2.2525417911510261410205347e+01, 2.2803007488961426903566920e+01, 2.3084114219397521301316111e+01, 2.3368970310885331538480179e+01, 2.3657834409456066774035863e+01, 2.3950996123628848479052048e+01, 2.4248781608052614781871560e+01, 2.4551560528643458715087857e+01, 2.4859754855145844760826474e+01, 2.5173850108958237208064469e+01, 2.5494409967825708622699690e+01, 2.5822095551225733525384449e+01, 2.6157691379788251433263128e+01, 2.6502141097849996836684948e+01, 2.6856597908134813373229728e+01, 2.7222497961270095541841192e+01, 2.7601671075507827784986288e+01, 2.7996515289006133611177657e+01, 2.8410287565216026379953973e+01, 2.8847623300317097284912261e+01, 2.9315556495648897392258304e+01, 2.9825809458081402413142885e+01, 3.0401117779657764294819147e+01, 3.1100951037096511748801284e+01};
  static NumericalScalar weights[128] = {7.7880553112849072198766294e-02, 7.4943512919414536910806816e-02, 6.9397141749149403969696237e-02, 6.1836746879648318333389484e-02, 5.3020239572973708333178304e-02, 4.3743820721247411296800373e-02, 3.4726352391828770445416924e-02, 2.6524913163345550162895103e-02, 1.9493216056892157244261750e-02, 1.3782486012401120501189296e-02, 9.3748300171492664629503913e-03, 6.1343092263553692729419654e-03, 3.8610584455446167914723509e-03, 2.3375177719222513086212317e-03, 1.3610623554229154160050607e-03, 7.6214868374154758836225128e-04, 4.1039424914329114912560963e-04, 2.1248156236438935169486303e-04, 1.0576827089812413493907134e-04, 5.0612366034536501431668631e-05, 2.3279572286335517177399318e-05, 1.0291000375452160655354603e-05, 4.3716822258132265765633340e-06, 1.7843872447157496838756722e-06, 6.9970863752445993319645415e-07, 2.6355296294457982485801659e-07, 9.5339500076719088603559665e-08, 3.3117736286643680141438096e-08, 1.1044763292458927772742952e-08, 3.5357534962409985484084265e-09, 1.0863147662329805097318140e-09, 3.2025234569583239758235623e-10, 9.0573898119730292539244887e-11, 2.4569445682930553563482259e-11, 6.3910595568724711910845065e-12, 1.5938054964152907343039923e-12, 3.8095919232928330906238773e-13, 8.7255716548816521833151253e-14, 1.9145600624729613090422991e-14, 4.0233405909344399451473717e-15, 8.0951675009613752719948473e-16, 1.5590499798725125935141981e-16, 2.8731516823222887474066625e-17, 5.0650721565888260826310931e-18, 8.5388712150794321468581963e-19, 1.3761221211258663377847429e-19, 2.1193549695588593255142755e-20, 3.1180535562838082396978111e-21, 4.3806011311093044092563723e-22, 5.8746940037473806331850174e-23, 7.5173209054033447555417709e-24, 9.1745690698535897008652337e-25, 1.0674922835552105657158543e-25, 1.1836003268604515384321003e-26, 1.2499832905811924753397716e-27, 1.2567593992729679307393964e-28, 1.2023503699854981391237349e-29, 1.0939938866854915092942988e-30, 9.4616999410627741962246371e-32, 7.7741057498528684818055696e-33, 6.0646535975418060617176232e-34, 4.4892461173993180351676461e-35, 3.1512300703857921867014789e-36, 2.0962490291314412221073794e-37, 1.3205879942102419576039463e-38, 7.8731309724410339834852054e-40, 4.4387936139837483446171576e-41, 2.3647681386310266830599492e-42, 1.1895249269031468696051071e-43, 5.6449598528648601040867566e-45, 2.5250874009530786161544321e-46, 1.0637272217126616433515017e-47, 4.2161469063242967027862854e-49, 1.5707547975689386981401980e-50, 5.4949672861369168167697578e-52, 1.8031127826300551915850409e-53, 5.5436741208780826872988904e-55, 1.5950823588115267095636623e-56, 4.2899178785744838761108310e-58, 1.0770598014915502881777703e-59, 2.5210019210640023596252402e-61, 5.4933568192556171804844677e-63, 1.1127348357056033046953705e-64, 2.0919910037640666023889327e-66, 3.6444488891998227750903798e-68, 5.8729939242428208168494011e-70, 8.7388392883152991177346810e-72, 1.1983435235137524938450365e-73, 1.5113381294896973696530902e-75, 1.7492932046071090072619502e-77, 1.8539433227865165645693191e-79, 1.7947949751038629650034721e-81, 1.5830825023894390544815014e-83, 1.2687510116986886750192806e-85, 9.2123101777701364185032765e-88, 6.0412782427518176337230762e-90, 3.5662449072374121768733826e-92, 1.8882726818582926654010717e-94, 8.9335232334505241260528038e-97, 3.7608844095106074694647086e-99, 1.4025800177880556273945875e-101, 4.6114419052210857911207097e-104, 1.3296527917278135803395156e-106, 3.3431060612647631418663216e-109, 7.2838408325905858803792272e-112, 1.3658255130442080104321089e-114, 2.1876645330156469909347131e-117, 2.9682322505121909735883514e-120, 3.3800825692122852526903147e-123, 3.1972924782982543066350585e-126, 2.4832856265123329861700072e-129, 1.5630453427246388490165028e-132, 7.8550152086622435117816579e-136, 3.0983962164454199109222893e-139, 9.4051577447557691199185888e-143, 2.1469376259019048933868626e-146, 3.5864591066294724118391333e-150, 4.2436752264252758983575152e-154, 3.4184301509636211134786682e-158, 1.7847924896743181968557422e-162, 5.6754752717640951349146435e-167, 1.0136208941909344983031939e-171, 9.1144573618340727008462260e-177, 3.5359397081504360617642767e-182, 4.6913583190766486697804951e-188, 1.4509334500270392274530570e-194, 5.0208177568884358829526795e-202, 2.9540145871800834681710394e-211};
  NumericalScalar value(0.0);
  // Least square approximation of eZ1
  const NumericalScalar z0(-1.5270815222604243733 - 0.25091814704012410653 * log(n));
  for (UnsignedLong i = 0; i < 128; ++i)
    value += weights[i] * ((z0 + nodes[i]) * exp(-z0 * nodes[i]) * pow(pNormal(z0 + nodes[i], true), n - 1) + (z0 - nodes[i]) * exp(z0 * nodes[i]) * pow(pNormal(z0 - nodes[i], true), n - 1));
  return n * exp(-0.5 * z0 * z0) * value;
}


/* Dickey Fuller asymptotic table
   The use of such table allow to make a linear approximation
*/
NumericalScalar DistFunc::pDickeyFullerTrend(const NumericalScalar x,
                                             Bool tail)
{
  // Quantiles and values available
  const NumericalScalar q001(-3.96);
  const NumericalScalar q005(-3.41);
  const NumericalScalar q010(-3.13);
  const NumericalScalar p001(0.01);
  const NumericalScalar p005(0.05);
  const NumericalScalar p010(0.10);
  if (x < q001)
    {
      LOGWARN(OSS() <<  "Warning! Result p-value is missing. The return result is the 0.01 quantile level ");
      return p001;
    }

  if ((x >= q001) && (x <= q005) )
    {
      // Linear approach
      // quantileFunction(q) = a * q + b
      // We use information available : quantile values for levels 0.01 and 0.05
      const NumericalScalar a((p005 - p001) / (q005 - q001));
      const NumericalScalar b(p001 - a * q001);
      return (a * x + b);
    }

  if ((x > q005) && (x <= q010) )
    {
      // Linear approach using the same approach
      // quantileFunction(q) = a * q + b
      // We use information available : quantile values for levels 0.05 and 0.10
      const NumericalScalar a((p010 - p005) / (q010 - q005));
      const NumericalScalar b(p005 - a * q005);
      return (a * x + b);
    }

  // Arbitrary we seek values until level 0.15
  if (x > q010)
    {
      LOGWARN(OSS() <<  "Warning! Result p-value is missing. The return result is the 0.10 quantile levels ");
      return p010;
    }

  // cannot happen
  return -1.0;
}

NumericalScalar DistFunc::pDickeyFullerConstant(const NumericalScalar x,
                                                Bool tail)
{
  // Quantiles and values available
  const NumericalScalar q001(-3.43);
  const NumericalScalar q005(-2.86);
  const NumericalScalar q010(-2.57);
  const NumericalScalar p001(0.01);
  const NumericalScalar p005(0.05);
  const NumericalScalar p010(0.10);
  if (x < q001)
    {
      LOGWARN(OSS() <<  "Warning! Result p-value is missing. The return result is the 0.01 quantile level ");
      return p001;
    }

  if ((x >= q001) && (x <= q005) )
    {
      // Linear approach
      // quantileFunction(q) = a * q + b
      // We use information available : quantile values for levels 0.01 and 0.05
      const NumericalScalar a((p005 - p001) / (q005 - q001));
      const NumericalScalar b(p001 - a * q001);
      return (a * x + b);
    }

  if ((x > q005) && (x <= q010) )
    {
      // Linear approach using the same approach
      // quantileFunction(q) = a * q + b
      // We use information available : quantile values for levels 0.05 and 0.10
      const NumericalScalar a((p010 - p005) / (q010 - q005));
      const NumericalScalar b(p005 - a * q005);
      return (a * x + b);
    }

  // Arbitrary we seek values until level 0.15
  if (x > q010)
    {
      LOGWARN(OSS() <<  "Warning! Result p-value is missing. The return result is the 0.10 quantile levels ");
      return p010;
    }

  // cannot happen
  return -1.0;
}

NumericalScalar DistFunc::pDickeyFullerNoConstant(const NumericalScalar x,
                                                  Bool tail)
{
  // Quantiles and values available
  const NumericalScalar q001(-2.57);
  const NumericalScalar q005(-1.94);
  const NumericalScalar q010(-1.62);
  const NumericalScalar p001(0.01);
  const NumericalScalar p005(0.05);
  const NumericalScalar p010(0.10);
  if (x < q001)
    {
      LOGWARN(OSS() <<  "Warning! Result p-value is missing. The return result is the 0.01 quantile level ");
      return p001;
    }

  if ((x >= q001) && (x <= q005) )
    {
      // Linear approach
      // quantileFunction(q) = a * q + b
      // We use information available : quantile values for levels 0.01 and 0.05
      const NumericalScalar a((p005 - p001) / (q005 - q001));
      const NumericalScalar b(p001 - a * q001);
      return (a * x + b);
    }

  if ((x > q005) && (x <= q010) )
    {
      // Linear approach using the same approach
      // quantileFunction(q) = a * q + b
      // We use information available : quantile values for levels 0.05 and 0.10
      const NumericalScalar a((p010 - p005) / (q010 - q005));
      const NumericalScalar b(p005 - a * q005);
      return (a * x + b);
    }

  // Arbitrary we seek values until level 0.15
  if (x > q010)
    {
      LOGWARN(OSS() <<  "Warning! Result p-value is missing. The return result is the 0.10 quantile levels ");
      return p010;
    }

  // cannot happen
  return -1.0;
}

// We currently use the asymptotic distribution
// We get the values for levels 0.01, 0.05 and 0.10
// If level is lower than 0.01 we return the 0.01 quantile value and we inform user
// by giving a warning message
// if the quantile level is between 0.01 and 0.05, we use a linear interpolation
// The same result is done if the level seeked is between 0.05 and 0.10
// Finally if the leval is upper than 0.10 we return the 0.10-quantile value with a warning message
NumericalScalar DistFunc::qDickeyFullerTrend(const NumericalScalar q,
                                             Bool tail)
{
  // Asymptotic distribution
  // quantile values for levels 0.01, 0.05 and 0.10
  const NumericalScalar q001(-4.96);
  const NumericalScalar q005(-3.41);
  const NumericalScalar q010(-3.13);
  if (q < 0.01)
    {
      LOGWARN(OSS() <<  "Warning! Result quantile value is missing. The return result is for level 0.01 ");
      return q001;
    }

  if ((q >= 0.01) && (q <= 0.05) )
    {
      // Linear approach
      // quantileFunction(q) = a * q + b
      // We use information available : quantile values for levels 0.01 and 0.05
      const NumericalScalar a((q005 - q001) / (0.05 - 0.01));
      const NumericalScalar b(q001 - a * 0.01);
      return (a * q + b);
    }

  if ((q > 0.05) && (q <= 0.10) )
    {
      // Linear approach using the same approach
      // quantileFunction(q) = a * q + b
      // We use information available : quantile values for levels 0.05 and 0.10
      const NumericalScalar a((q010 - q005) / (0.10 - 0.05));
      const NumericalScalar b(q005 - a * 0.05);
      return (a * q + b);
    }

  // Arbitrary we seek values until level 0.15
  if ((q > 0.10) && (q <= 0.15))
    {
      LOGWARN(OSS() <<  "Warning! Result quantile value is missing. The return result is for level 0.10 ");
      return q010;
    }

  else
    throw InvalidArgumentException(HERE) << "Can not give quantile value for the level " << q << ". Value is missing in table";
}

NumericalScalar DistFunc::qDickeyFullerConstant(const NumericalScalar q,
                                                Bool tail)
{
  // Asymptotic distribution
  // quantile values for levels 0.01, 0.05 and 0.10
  const NumericalScalar q001(-3.43);
  const NumericalScalar q005(-2.86);
  const NumericalScalar q010(-2.57);
  if (q < 0.01)
    {
      LOGWARN(OSS() <<  "Warning! Result quantile value is missing. The return result is for level 0.01 ");
      return q001;
    }

  if ((q >= 0.01) && (q <= 0.05) )
    {
      // Linear approach
      // quantileFunction(q) = a * q + b
      // We use information available : quantile values for levels 0.01 and 0.05
      const NumericalScalar a((q005 - q001) / (0.05 - 0.01));
      const NumericalScalar b(q001 - a * 0.01);
      return (a * q + b);
    }

  if ((q > 0.05) && (q <= 0.10) )
    {
      // Linear approach using the same approach
      // quantileFunction(q) = a * q + b
      // We use information available : quantile values for levels 0.05 and 0.10
      const NumericalScalar a((q010 - q005) / (0.10 - 0.05));
      const NumericalScalar b(q005 - a * 0.05);
      return (a * q + b);
    }

  // Arbitrary we seek values until level 0.15
  if ((q > 0.10) && (q <= 0.15))
    {
      LOGWARN(OSS() <<  "Warning! Result quantile value is missing. The return result is for level 0.10 ");
      return q010;
    }

  else
    throw InvalidArgumentException(HERE) << "Can not give quantile value for the level " << q << ". Value is missing in table";
}

NumericalScalar DistFunc::qDickeyFullerNoConstant(const NumericalScalar q,
                                                  Bool tail)
{
  // Asymptotic distribution
  // quantile values for levels 0.01, 0.05 and 0.10
  const NumericalScalar q001(-2.57);
  const NumericalScalar q005(-1.94);
  const NumericalScalar q010(-1.62);
  if (q < 0.01)
    {
      LOGWARN(OSS() <<  "Warning! Result quantile value is missing. The return result is for level 0.01 ");
      return q001;
    }

  if ((q >= 0.01) && (q <= 0.05) )
    {
      // Linear approach
      // quantileFunction(q) = a * q + b
      // We use information available : quantile values for levels 0.01 and 0.05
      const NumericalScalar a((q005 - q001) / (0.05 - 0.01));
      const NumericalScalar b(q001 - a * 0.01);
      return (a * q + b);
    }

  if ((q > 0.05) && (q <= 0.10) )
    {
      // Linear approach using the same approach
      // quantileFunction(q) = a * q + b
      // We use information available : quantile values for levels 0.05 and 0.10
      const NumericalScalar a((q010 - q005) / (0.10 - 0.05));
      const NumericalScalar b(q005 - a * 0.05);
      return (a * q + b);
    }

  // Arbitrary we seek values until level 0.15
  if ((q > 0.10) && (q <= 0.15))
    {
      LOGWARN(OSS() <<  "Warning! Result quantile value is missing. The return result is for level 0.10 ");
      return q010;
    }

  else
    throw InvalidArgumentException(HERE) << "Can not give quantile value for the level " << q << ". Value is missing in table";
}

END_NAMESPACE_OPENTURNS